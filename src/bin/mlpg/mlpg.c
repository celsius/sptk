/************************************************************************
*									*
*    ML-based Parameter Generation from PDFs	 			*
*									*
*					1999.7 T.Masuko			*
*									*
*	usage:								*
*		mlpg [ options ] [infile] > stdout			*
*	options:							*
*		-m m  : order of vector                 [25]		*
*		-l l  : length of vector                [m-1]		*
*		-d fn : filename of delta coefficients  [N/A]		*
*		-i    : input (mU^{-1}, U^{-1})         [FALSE]		*
*		-s s  : range of influenced frames      [30]		*
*		-h    : print this message				*
*	infile:								*
*		PDF sequence						*
*			, m(0), m(1), ..., m((m+1)*d-1),		*
*			  U(0), U(1), ..., U((m+1)*d-1),		*
*	stdout:								*
*		parameter sequence					*
*			, c(0), c(1), ..., c_1(m),			*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/* default values */
#define		ORDER		25
#define		MTYPE		FA
#define		RANGE		30


/*  Command Name  */
char		*cmnd;


/*  Other Definitions  */
#ifdef DOULBE
	typedef double real;
#else
	typedef float real;
#endif

#define	LENGTH		256
#define	INFTY		((double) 1.0e+38)
#define	INVINF		((double) 1.0e-38)

#define	WLEFT		0
#define	WRIGHT		1

#define	abs(x)		((x) > 0.0 ? (x) : -(x))
#define	sign(x)		((x) > 0.0 ? 1 : ((x) < 0.0 ? -1 : 0))
#define	finv(x)		(abs(x) <= INVINF ? sign(x)*INFTY : 1.0/(x))
#define	min(x, y)	((x) < (y) ? (x) : (y))

typedef struct _DWin {
	int		num;		/* number of static + deltas */
	char		**fn;		/* delta window coefficient file */
	int		**width;	/* width [0..num-1][0(left) 1(right)] */
	double		**coef;		/* coefficient [0..num-1][length[0]..length[1]] */
	int		maxw[2];	/* max width [0(left) 1(right)] */
} DWin;

typedef struct _SMatrix {
	double		**mseq;		/* sequence of mean vector */
	double		**ivseq;	/* sequence of invarsed variance vector */
	double		*ivar0;		/* initial inversed variance vector */
	double		***P;		/* matrix P[th][tv][m] */
	double		**c;		/* parameter c */
	double		**cc;
	double		***pi;
	double		***nu;
	double		***kappa;
	double		***K;
	int		t;		/* time index */
	int		length;		/* matrix length (must be power of 2) */
	unsigned int	mask;		/* length - 1 */
} SMatrix;

typedef struct _PStream {
	int		vSize;		/* data vector size */
	int		order;		/* order of cepstrum */
	int		range;
	DWin		dw;
	double		*mean;		/* input mean vector */
	double		*ivar;		/* input inversed variance vector */
	double		*par;		/* output parameter vector */
	Boolean		mType;		/* type of input mean vector (0:m 1:mU^{-1}) */
	SMatrix		sm;
} PStream;



void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - obtain parameter sequence from PDF sequence\n", cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [infile] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of vector                 [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : length of vector                [m+1]\n");
    fprintf(stderr, "       -d fn : filename of delta coefficients  [N/A]\n");
    fprintf(stderr, "       -i    : input (mU^{-1}, U^{-1})         [%s]\n", BOOL[MTYPE]);
    fprintf(stderr, "       -s s  : range of influenced frames      [%d]\n", RANGE);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       PDF sequence                           [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       parameter sequence\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       Option '-d' may be repeated to use multiple\n");
    fprintf(stderr, "       delta parameters.\n");
    fprintf(stderr, "       If 'fn' is an integer, delta coefficients are\n");
    fprintf(stderr, "       calculated based on a regression formula.\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
	char		*s;
	FILE		*pdffp = stdin, *parfp = stdout;
	int		nframe, delay;
	PStream		pst;
	register int	i;
	void		InitPStream(PStream *);
	double		*mlpg(PStream *);

	pst.order = ORDER;
	pst.range = RANGE;
	pst.mType = MTYPE;
	pst.dw.fn = (char **)calloc(sizeof(char *), argc);
	pst.dw.num = 1;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
	else
		cmnd++;

	while (--argc) {
		if (**++argv == '-') {
			switch (*(*argv+1)) {
				case 'd':
					pst.dw.fn[pst.dw.num] = *++argv;
					pst.dw.num++;
					--argc;
					break;
				case 'm':
					pst.order = atoi(*++argv);
					--argc;
					break;
				case 'l':
					pst.order = atoi(*++argv) - 1;
					--argc;
					break;
				case 'i':
					pst.mType = 1 - pst.mType;
					break;
				case 'r':
					pst.range = atoi(*++argv);
					--argc;
					break;
				case 'h':
					usage(0);
				default:
					fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
					usage(1);
				}
		}
		else
			if ((pdffp = fopen(*argv, "r")) == NULL){
				fprintf(stderr, "%s: Can't open '%s'!\n", cmnd, *argv);
				exit(2);
			}
	}

	InitPStream(&pst);

	delay = pst.range + pst.dw.maxw[WRIGHT];
	nframe = 0;
	while (freadf(pst.mean, sizeof(*(pst.mean)), pst.vSize * 2, pdffp) == pst.vSize * 2) {
		if (pst.dw.num == 1)
			fwritef(pst.mean, sizeof(*pst.mean), pst.order + 1, parfp);
		else {
			if (!pst.mType)
				for (i = 0; i < pst.vSize; i++)
					pst.ivar[i] = finv(pst.ivar[i]);
			mlpg(&pst);
			if (nframe >= delay)
				fwritef(pst.par, sizeof(*(pst.par)), pst.order + 1, parfp);
		}
		nframe++;
	}

	if (pst.dw.num > 1) {
		for (i = 0; i < pst.vSize; i++) {
			pst.mean[i] = 0.0;
			pst.ivar[i] = 0.0;
		}
		for (i = 0; i < min(nframe, delay); i++) {
			mlpg(&pst);
			fwritef(pst.par, sizeof(*(pst.par)), pst.order + 1, parfp);
		}
	}

	exit(0);
}


void InitPStream(PStream *pst)
{
	void		InitDWin(PStream *);
	double		*dcalloc(int);
	double		**ddcalloc(int, int);
	double		***dddcalloc(int, int, int, int, int, int);
	int		half, full;
	register int	i, m;

	InitDWin(pst);

	half = pst->range * 2;
	full = pst->range * 4 + 1;

	pst->vSize	= (pst->order + 1) * pst->dw.num;

	pst->sm.length = LENGTH;
	while (pst->sm.length < pst->range + pst->dw.maxw[WRIGHT])
		pst->sm.length *= 2;

	pst->mean	= dcalloc(pst->vSize*2);
	pst->ivar	= pst->mean + pst->vSize;

	pst->sm.mseq	= ddcalloc(pst->sm.length, pst->vSize);
	pst->sm.ivseq	= ddcalloc(pst->sm.length, pst->vSize);
	pst->sm.ivar0	= dcalloc(pst->vSize);

	pst->sm.c	= ddcalloc(pst->sm.length, pst->order+1);
	pst->sm.cc	= ddcalloc(pst->sm.length, pst->order+1);

	pst->sm.P	= dddcalloc(full,        pst->sm.length, pst->order+1, half, 0, 0);
	pst->sm.pi	= dddcalloc(full,        pst->dw.num,    pst->order+1, half, 0, 0);
	pst->sm.nu	= dddcalloc(pst->dw.num, pst->dw.num,    pst->order+1, 0,    0, 0);
	pst->sm.kappa	= dddcalloc(pst->dw.num, pst->dw.num,    pst->order+1, 0,    0, 0);
	pst->sm.K	= dddcalloc(full,        pst->dw.num,    pst->order+1, half, 0, 0);

	for (i = 0; i < pst->sm.length; i++)
		for (m = 0; m < pst->vSize; m++)
			pst->sm.ivseq[i][m] = 0.0;
	for (i = 0; i < pst->sm.length; i++)
		for (m = 0; m <= pst->order; m++)
			pst->sm.P[0][i][m] = 0.0;

	pst->sm.t = pst->range - 1;
	pst->sm.mask = pst->sm.length - 1;
}


void InitDWin(PStream *pst)
{
	register int	i, j, k;
	int		fsize, leng, isint, denom;
	double		x;
	FILE		*fp;

	/* memory allocation */
	if ((pst->dw.width = (int **) calloc(pst->dw.num, sizeof(int *))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	for (i = 0; i < pst->dw.num; i++)
		if ((pst->dw.width[i] = (int *) calloc(2, sizeof(int))) == NULL) {
			fprintf(stderr, "Cannot Allocate Memory\n");
			exit(1);
		}
	if ((pst->dw.coef = (double **) calloc(pst->dw.num, sizeof(double *))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}

	/* window for static parameter */
	pst->dw.width[0][WLEFT] = pst->dw.width[0][WRIGHT] = 0;
	pst->dw.coef[0] = (double *) calloc(1, sizeof(**(pst->dw.coef)));
	pst->dw.coef[0][0] = 1;

	/* set delta coefficients */
	for (i = 1; i < pst->dw.num; i++) {
		for (j = 0, isint = 1; j < strlen(pst->dw.fn[i]); j++) {
			isint = isint && (pst->dw.fn[i][j] >= '0') && (pst->dw.fn[i][j] <= '9');
		}
		if (isint) {					/* calculate regression coefficients */
			leng = atoi(pst->dw.fn[i]);
			if (leng < 1) {
				fprintf(stderr, "illegal condition for regression formula\n");
				exit(1);
			}
			pst->dw.width[i][WLEFT] = -leng + pst->dw.width[i-1][WLEFT];
			pst->dw.width[i][WRIGHT] = leng + pst->dw.width[i-1][WRIGHT];
			fsize = -pst->dw.width[i][WLEFT] + pst->dw.width[i][WRIGHT] + 1;
			pst->dw.coef[i] = (double *) calloc(fsize, sizeof(**(pst->dw.coef)));
			pst->dw.coef[i] -= pst->dw.width[i][WLEFT];
			for (j = 1, denom = 0; j <= leng; j++) {
				denom += j * j * 2;
			}
			for (j = -leng; j <= leng; j++)
				for (k = pst->dw.width[i-1][WLEFT]; k <= pst->dw.width[i-1][WRIGHT]; k++)
					pst->dw.coef[i][j+k] += (double) j / denom * pst->dw.coef[i-1][k];
		}
		else {						/* read from file */
			if ((fp = fopen(pst->dw.fn[i], "r")) == NULL) {
				fprintf(stderr, "file %s not found\n", pst->dw.fn[i]);
				exit(1);
			}

			/* check the number of coefficients */
			fseek(fp, 0L, 2);
			fsize = ftell(fp) / sizeof(real);
			fseek(fp, 0L, 0);

			/* read coefficients */
			pst->dw.coef[i] = (double *) calloc(fsize, sizeof(**(pst->dw.coef)));
			freadf(pst->dw.coef[i], sizeof(**(pst->dw.coef)), fsize, fp);

			/* set pointer */
			leng = fsize / 2;
			pst->dw.coef[i] += leng;
			pst->dw.width[i][WLEFT] = -leng;
			pst->dw.width[i][WRIGHT] = leng;
			if (fsize % 2 == 0)
				pst->dw.width[i][WRIGHT]--;
		}
	}

	pst->dw.maxw[WLEFT] = pst->dw.maxw[WRIGHT] = 0;
	for (i = 0; i < pst->dw.num; i++) {
		if (pst->dw.maxw[WLEFT] > pst->dw.width[i][WLEFT])
			pst->dw.maxw[WLEFT] = pst->dw.width[i][WLEFT];
		if (pst->dw.maxw[WRIGHT] < pst->dw.width[i][WRIGHT])
			pst->dw.maxw[WRIGHT] = pst->dw.width[i][WRIGHT];
	}
}


double *dcalloc(int x)
{
	double		*ptr;

	if ((ptr = (double *) calloc(x, sizeof(*ptr))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	return(ptr);
}


double **ddcalloc(int x, int y)
{
	register int	i;
	double		**ptr;

	if ((ptr = (double **) calloc(x, sizeof(*ptr))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	for (i = 0; i < x; i++)
		ptr[i] = dcalloc(y);
	return(ptr);
}


double ***dddcalloc(int x, int y, int z, int xoff, int yoff, int zoff)
{
	double		***ptr;
	register int	i, j;

	if ((ptr = (double ***) calloc(x, sizeof(*ptr))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	for (i = 0; i < x; i++) {
		if ((ptr[i] = (double **) calloc(y, sizeof(**ptr))) == NULL) {
			fprintf(stderr, "Cannot Allocate Memory\n");
			exit(1);
		}
		for (j = 0; j < y; j++) {
			if ((ptr[i][j] = (double *) calloc(z, sizeof(***ptr))) == NULL) {
				fprintf(stderr, "Cannot Allocate Memory\n");
				exit(1);
			}
			ptr[i][j] += zoff;
		}
		ptr[i] += yoff;
	}
	ptr += xoff;
	return(ptr);
}


double *mlpg(PStream *pst)
{
	void		calc_pi(PStream *);
	void		calc_nu(PStream *);
	void		calc_kappa(PStream *);
	void		inv_kappa(PStream *);
	void		calc_K(PStream *);
	void		calc_cc(PStream *);
	void		update_c(PStream *);
	void		update_P(PStream *);
	int		tcur, tmin, tmax;
	register int	m, u;

	pst->sm.t++;
	tcur = pst->sm.t & pst->sm.mask;
	tmin = (pst->sm.t - pst->range) & pst->sm.mask;
	tmax = (pst->sm.t + pst->dw.maxw[WRIGHT]) & pst->sm.mask;

	for (u = -pst->range*2; u <= pst->range*2; u++) {
		for (m = 0; m <= pst->order; m++)
			pst->sm.P[u][tmax][m] = 0.0;
	}
	for (m = 0; m < pst->vSize; m++) {
		pst->sm.mseq[tmax][m] = pst->mean[m];
		pst->sm.ivseq[tmax][m] = pst->ivar[m];
	}
	for (m = 0; m <= pst->order; m++) {
		if (!pst->mType)
			pst->sm.c[tmax][m] = pst->mean[m];
		else
			pst->sm.c[tmax][m] = pst->mean[m] * finv(pst->ivar[m]);
		pst->sm.P[0][tmax][m] = finv(pst->ivar[m]);
		pst->sm.ivar0[m] = pst->sm.ivseq[tcur][m];
	}

	calc_pi(pst);
	calc_nu(pst);
	calc_kappa(pst);
	inv_kappa(pst);
	calc_K(pst);
	calc_cc(pst);
	update_c(pst);
	update_P(pst);
	pst->par = pst->sm.c[tmin];
	return(pst->par);
}


void calc_pi(PStream *pst)
{
	register int	i, j, m, u;

	for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
		for (i = 0; i < pst->dw.num; i++)
			for (m = 0; m <= pst->order; m++) {
				pst->sm.pi[u][i][m] = 0.0;
				for (j = pst->dw.width[i][WLEFT]; j <= pst->dw.width[i][WRIGHT]; j++)
					pst->sm.pi[u][i][m] += pst->sm.P[u-j][(pst->sm.t+j)&pst->sm.mask][m] * pst->dw.coef[i][j];
			}
}


void calc_nu(PStream *pst)
{
	register int	i, j, k, m;

	for (i = 0; i < pst->dw.num; i++)
		for (j = 0; j < pst->dw.num; j++)
			for (m = 0; m <= pst->order; m++) {
				pst->sm.nu[i][j][m] = 0.0;
				for (k = pst->dw.width[i][WLEFT]; k <= pst->dw.width[i][WRIGHT]; k++)
					pst->sm.nu[i][j][m] += pst->dw.coef[i][k] * pst->sm.pi[k][j][m];
			}
}


void calc_kappa(PStream *pst)
{
	double		*ivar1, *ivar0;
	register int	i, j, m, n;
	double		x;

	ivar1 = pst->sm.ivseq[pst->sm.t & pst->sm.mask];
	ivar0 = pst->sm.ivar0;
	for (i = n = 0; i < pst->dw.num; i++, n += pst->order+1)
		for (j = 0; j < pst->dw.num; j++)
			for (m = 0; m <= pst->order; m++) {
				x = ivar1[n+m] - ivar0[n+m];
				if (i == j)
					pst->sm.kappa[i][j][m] = 1.0;
				else
					pst->sm.kappa[i][j][m] = 0.0;
				pst->sm.kappa[i][j][m] += x * pst->sm.nu[i][j][m];
			}
}


void inv_kappa(PStream *pst)
{
	register int	i, j, k, m;
	double		t, u, det;

	for (m = 0; m <= pst->order; m++) {
		det = 1;
		for (k = 0; k < pst->dw.num; k++) {
			t = pst->sm.kappa[k][k][m];
			det *= t;
			for (j = 0; j < pst->dw.num; j++)
				pst->sm.kappa[k][j][m] /= t;
			pst->sm.kappa[k][k][m] = 1 / t;
			for (j = 0; j < pst->dw.num; j++)
				if (j != k) {
					u = pst->sm.kappa[j][k][m];
					for (i = 0; i < pst->dw.num; i++)
						if (i != k)
							pst->sm.kappa[j][i][m] -= pst->sm.kappa[k][i][m] * u;
						else
							pst->sm.kappa[j][i][m] = -u / t;
				}
		}
	}
}


void calc_K(PStream *pst)
{
	register int	i, j, m, u;

	for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
		for (i = 0; i < pst->dw.num; i++)
			for (m = 0; m <= pst->order; m++) {
				pst->sm.K[u][i][m] = 0.0;
				for (j = 0; j < pst->dw.num; j++)
					pst->sm.K[u][i][m] += pst->sm.pi[u][j][m] * pst->sm.kappa[j][i][m];
			}
}


void calc_cc(PStream *pst)
{
	register int	i, j, m, n, t, u;
	double		*ivar1, *ivar0, *mean1, *mean0;
	double		x1, x0;

	t = pst->sm.t;
	ivar1 = pst->sm.ivseq[t & pst->sm.mask];
	ivar0 = pst->sm.ivar0;
	mean1 = mean0 = pst->sm.mseq[t & pst->sm.mask];
	for (m = 0; m <= pst->order; m++) {
		for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++) {
			pst->sm.cc[(t+u)&pst->sm.mask][m] = 0.0;
			for (i = n = 0; i < pst->dw.num; i++, n += pst->order+1) {
				if (!pst->mType) {
					x1 = mean1[n+m];
					if (i == 0)
						x0 = mean0[n+m];
					else
						x0 = 0.0;
				}
				else
					x1 = x0 = 0.0;
				for (j = pst->dw.width[i][WLEFT]; j <= pst->dw.width[i][WRIGHT]; j++) {
					x1 -= pst->dw.coef[i][j] * pst->sm.c[(t+j)&pst->sm.mask][m];
					if (i == 0)
						x0 -= pst->dw.coef[i][j] * pst->sm.c[(t+j)&pst->sm.mask][m];
				}
				x1 *= ivar1[n+m];
				if (i == 0)
					x0 *= ivar0[n+m];
				if (pst->mType) {
					x1 += mean1[n+m];
					if (i == 0)
						x0 += mean0[n+m];
				}
				pst->sm.cc[(t+u)&pst->sm.mask][m] += pst->sm.K[u][i][m] * (x1 - x0);
			}
		}
	}
}


void update_c(PStream *pst)
{
	register int	m, u;

	for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
		for (m = 0; m <= pst->order; m++)
			pst->sm.c[(pst->sm.t+u)&pst->sm.mask][m] += pst->sm.cc[(pst->sm.t+u)&pst->sm.mask][m];
}


void update_P(PStream *pst)
{
	register int	i, m, u, v, n;
	double		*ivar1, *ivar0;
	double		x;

	ivar1 = pst->sm.ivseq[pst->sm.t & pst->sm.mask];
	ivar0 = pst->sm.ivar0;
	for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
		for (v = u; v <= pst->dw.maxw[WRIGHT]; v++)
			for (i = n = 0; i < pst->dw.num; i++, n += pst->order+1)
				for (m = 0; m <= pst->order; m++) {
					x = ivar1[n+m] - ivar0[n+m];
					pst->sm.P[-u+v][(pst->sm.t+u)&pst->sm.mask][m] -= pst->sm.K[u][i][m] * x * pst->sm.pi[v][i][m];
					if (v != u)
						pst->sm.P[-v+u][(pst->sm.t+v)&pst->sm.mask][m] = pst->sm.P[-u+v][(pst->sm.t+u)&pst->sm.mask][m];
				}
}

