/************************************************************************************************
*												*
*    Random Parameter Generation from PDFs	 						*
*												*
*									2000.8  T.Masuko	*
*												*
*	usage:											*
*		rndpg [ options ] [infile] > stdout						*
*	options:										*
*		-m m              : order of vector                              [25]		*
*		-l l              : length of vector                             [m+1]		*
*		-d fn             : filename of delta coefficients               [N/A]		*
*		-d coef [coef...] : delta coefficients                           [N/A]		*
*		-r n w1 [w2]      : number and width of regression coefficients  [N/A]		*
*		-i i              : type of input PDFs                           [0]		*
*		-s s              : range of influenced frames                   [30]		*
*		-R                : generate random parameter sequence           [TRUE]		*
*		-h                : print this message						*
*	infile:											*
*		PDF sequence									*
*		ex.)	, m(0), m(1), ..., m((m+1)*d-1),					*
*			  U(0), U(1), ..., U((m+1)*d-1),					*
*	stdout:											*
*		parameter sequence								*
*			, c(0), c(1), ..., c_1(m),						*
*												*
************************************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/* default values */
#define		ORDER		25
#define		ITYPE		0
#define		RANGE		30
#define		RND		TR
#define		NPS		1


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
#define	INFTY2		((double) 1.0e+19)
#define	INVINF		((double) 1.0e-38)
#define	INVINF2		((double) 1.0e-19)

#define	WLEFT		0
#define	WRIGHT		1

#define	abs(x)		((x) > 0.0 ? (x) : -(x))
#define	sign(x)		((x) >= 0.0 ? 1 : -1)
#define	finv(x)		(abs(x) <= INVINF2 ? sign(x)*INFTY : (abs(x) >= INFTY2 ? 0 : 1.0/(x)))
#define	min(x, y)	((x) < (y) ? (x) : (y))

typedef struct _DWin {
	int		num;		/* number of static + deltas */
	int		calccoef;	/* calculate regression coefficients */
	char		**fn;		/* delta window coefficient file */
	int		**width;	/* width [0..num-1][0(left) 1(right)] */
	double		**coef;		/* coefficient [0..num-1][length[0]..length[1]] */
	int		maxw[2];	/* max width [0(left) 1(right)] */
} DWin;

typedef struct _SMatrix {
	double		**mseq;		/* sequence of mean vector */
	double		**ivseq;	/* sequence of invarsed variance vector */
	double		*ivar0;		/* initial inversed variance vector */
	double		***Q;		/* matrix Q[th][tv][m] */
	double		**c;		/* ML-parameter c[t][m] */
	double		***x;		/* randomized parameter x[t][nps][m] */
	double		***rnd;		/* random vector rnd[t][nps][m] */
	double		**w;
	double		**k;
	double		*alpha;
	int		t;		/* time index */
	int		length;		/* matrix length (must be power of 2) */
	unsigned int	mask;		/* length - 1 */
} SMatrix;

typedef struct _PStream {
	int		vSize;		/* data vector size */
	int		order;		/* order of cepstrum */
	int		range;
	int		nps;		/* number of randomized parameter sequence */
	DWin		dw;
	double		*mean;		/* input mean vector */
	double		*ivar;		/* input inversed variance vector */
	double		**par;		/* output parameter vectors */
	int		iType;		/* type of input PDFs */
					/*   0: ( m       , U      ) */
					/*   1: ( m       , U^{-1} ) */
					/*   2: ( mU^{-1} , U^{-1} ) */
	Boolean		rnd;
	SMatrix		sm;
} PStream;


double bmrnd(void);


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - obtain parameter sequence from PDF sequence\n", cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [infile] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m              : order of vector                              [%d]\n", ORDER);
    fprintf(stderr, "       -l l              : length of vector                             [m+1]\n");
    fprintf(stderr, "       -d fn             : filename of delta coefficients               [N/A]\n");
    fprintf(stderr, "       -d coef [coef...] : delta coefficients                           [N/A]\n");
    fprintf(stderr, "       -r n t1 [t2]      : number and width of regression coefficients  [N/A]\n");
    fprintf(stderr, "       -i i              : type of input PDFs                           [%d]\n", ITYPE);
    fprintf(stderr, "                             0: ( m       , U      )\n");
    fprintf(stderr, "                             1: ( m       , U^{-1} )\n");
    fprintf(stderr, "                             2: ( mU^{-1} , U^{-1} )\n");
    fprintf(stderr, "       -s s              : range of influenced frames                   [%d]\n", RANGE);
    fprintf(stderr, "       -R                : generate random parameter sequence           [%s]\n", BOOL[RND]);
    fprintf(stderr, "       -h                : print this message\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       PDF sequence                                                     [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       parameter sequence\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       1) Option '-d' may be repeated to use multiple delta parameters.\n");
    fprintf(stderr, "       2) Options '-d' and '-r' shuold not be defined simultaneously.\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
	char		*s;
	FILE		*pdffp = stdin, *parfp = stdout;
	int		nframe, delay;
	double		*par;
	char		*coef;
	int		coeflen;
	PStream		pst;
	register int	i, j;
	void		InitPStream(PStream *);
	double		*rndpg(PStream *);

	pst.order = ORDER;
	pst.range = RANGE;
	pst.iType = ITYPE;
	pst.rnd   = RND;
	pst.nps   = NPS;
	pst.dw.fn = (char **)calloc(sizeof(char *), argc);
	pst.dw.num = 1;
	pst.dw.calccoef = -1;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
	else
		cmnd++;

	while (--argc) {
		if (**++argv == '-') {
			switch (*(*argv+1)) {
				case 'd':
					if (pst.dw.calccoef == 1) {
						fprintf(stderr, "Options '-r' and '-d' should not be defined simultaneously.\n");
						exit(1);
					}
					pst.dw.calccoef = 0;
					if (isfloat(*++argv)) {
						coeflen = 0;
						for (i = 0; (i < argc - 1) && isfloat(argv[i]); i++) {
							coeflen += strlen(argv[i]) + 1;
						}
						coeflen += 1;
						coef = pst.dw.fn[pst.dw.num] = (char *)calloc(coeflen, sizeof(char));
						for (j = 0; j < i; j++) {
							sprintf(coef, " %s", *argv);
							coef += strlen(*argv) + 1;
							if (j < i-1) {
								argv++;
								argc--;
							}
						}
					}
					else {
						pst.dw.fn[pst.dw.num] = *argv;
					}
					pst.dw.num++;
					--argc;
					break;
				case 'r':
					if (pst.dw.calccoef == 0) {
						fprintf(stderr, "Options '-r' and '-d' should not be defined simultaneously.\n");
						exit(1);
					}
					pst.dw.calccoef = 1;
					coeflen = atoi(*++argv);
					--argc;
					if ((coeflen != 1) && (coeflen != 2)) {
						fprintf(stderr, "Number of delta parameter should be 1 or 2\n");
						exit(1);
					}
					if (argc <= 1) {
						fprintf(stderr, "Window size for delta parameter required.\n");
						exit(1);
					}
					pst.dw.fn[pst.dw.num] = *++argv;
					pst.dw.num++;
					--argc;
					if (coeflen == 2) {
						if (argc <= 1) {
							fprintf(stderr, "Window size for delta-delta parameter required.\n");
							exit(1);
						}
						pst.dw.fn[pst.dw.num] = *++argv;
						pst.dw.num++;
						--argc;
					}
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
					pst.iType = atoi(*++argv);
					--argc;
					break;
				case 's':
					pst.range = atoi(*++argv);
					--argc;
					break;
				case 'R':
					pst.rnd = 1 - pst.rnd;
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

	srand48((long)time(NULL));

	InitPStream(&pst);

	delay = pst.range + pst.dw.maxw[WRIGHT];
/*
	for (i = 0; i < pst.vSize; i++) {
		pst.mean[i] = 0.0;
		pst.ivar[i] = 0.0;
	}
	for (i = 0; i < delay; i++) {
		rndpg(&pst);
	}
*/
	nframe = 0;
	while (freadf(pst.mean, sizeof(*(pst.mean)), pst.vSize * 2, pdffp) == pst.vSize * 2) {
		if (pst.dw.num == 1)
			fwritef(pst.mean, sizeof(*pst.mean), pst.order + 1, parfp);
		else {
			if (pst.iType == 0)
				for (i = 0; i < pst.vSize; i++)
					pst.ivar[i] = finv(pst.ivar[i]);
			par = rndpg(&pst);
			if (nframe >= delay)
				fwritef(par, sizeof(*par), pst.order + 1, parfp);
		}
		nframe++;
	}

	if (pst.dw.num > 1) {
		for (i = 0; i < pst.vSize; i++) {
			pst.mean[i] = 0.0;
			pst.ivar[i] = 0.0;
		}
		for (i = 0; i < min(nframe, delay); i++) {
			par = rndpg(&pst);
			fwritef(par, sizeof(*par), pst.order + 1, parfp);
		}
	}

	exit(0);
}


int isfloat(char *c)
{
	int		isnum = 0, wfe = 1;
	register int	i = 0;

	if (strlen(c) == 0)
		return(0);

	if ((c[i] == '+') || (c[i] == '-'))
		i++;
	while ((c[i] >= '0') && (c[i] <= '9')) {
		isnum = 1;
		i++;
	}
	if (c[i] == '.') {
		i++;
		while ((c[i] >= '0') && (c[i] <= '9')) {
			isnum = 1;
			i++;
		}
	}
	if ((c[i] == 'e') || (c[i] == 'E')) {
		wfe = 0;
		i++;
		if ((c[i] == '+') || (c[i] == '-'))
			i++;
		while ((c[i] >= '0') && (c[i] <= '9')) {
			wfe = 1;
			i++;
		}
	}
	if ((c[i] == 'f') || (c[i] == 'F') || (c[i] == 'l') || (c[i] == 'L'))
		i++;

	if ((c[i] == '\0') && isnum && wfe)
		return(1);
	else
		return(0);
}


void InitPStream(PStream *pst)
{
	void		InitDWin(PStream *);
	double		*dcalloc(int, int);
	double		**ddcalloc(int, int, int, int);
	double		***dddcalloc(int, int, int, int, int, int);
	int		half, full;
	register int	i, j, m;


	InitDWin(pst);

	half = pst->range * 2;
	full = pst->range * 4 + 1;

	pst->vSize	= (pst->order + 1) * pst->dw.num;

	pst->sm.length = LENGTH;
	while (pst->sm.length < pst->range + pst->dw.maxw[WRIGHT])
		pst->sm.length *= 2;

	pst->mean	= dcalloc(pst->vSize*2, 0);
	pst->ivar	= pst->mean + pst->vSize;

	pst->sm.mseq	= ddcalloc(pst->sm.length, pst->vSize, 0, 0);
	pst->sm.ivseq	= ddcalloc(pst->sm.length, pst->vSize, 0, 0);
	pst->sm.ivar0	= dcalloc(pst->vSize, 0);

	pst->sm.c	= ddcalloc(pst->sm.length, pst->order+1, 0, 0);
	pst->sm.w	= ddcalloc(pst->range + pst->dw.maxw[WRIGHT] + 1, pst->order+1, 0, 0);
	pst->sm.w 	+= pst->range;
	pst->sm.k	= ddcalloc(pst->range + pst->dw.maxw[WRIGHT] + 1, pst->order+1, 0, 0);
	pst->sm.k 	+= pst->range;
	pst->sm.alpha	= dcalloc(pst->order+1, 0);

	pst->sm.x	= dddcalloc(pst->sm.length, pst->nps, pst->order+1, 0, 0, 0);
	pst->sm.rnd	= dddcalloc(pst->sm.length, pst->nps, pst->order+1, 0, 0, 0);

	for (i = 0; i < pst->sm.length; i++)
		for (j = 0; j < pst->nps; j++)
			for (m = 0; m <= pst->order; m++)
				pst->sm.rnd[i][j][m] = bmrnd();

	pst->sm.Q	= dddcalloc(full, pst->sm.length, pst->order+1, half, 0, 0);

	for (i = 0; i < pst->sm.length; i++)
		for (m = 0; m < pst->vSize; m++)
			pst->sm.ivseq[i][m] = 0.0;

	pst->sm.t = pst->range - 1;
	pst->sm.mask = pst->sm.length - 1;
}


void InitDWin(PStream *pst)
{
	double		*dcalloc(int, int);
	int		str2darray(char *, double **);
	register int	i, j;
	int		fsize, leng;
	double		x, s4, s2, s0;
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
	pst->dw.coef[0] = dcalloc(1, 0);
	pst->dw.coef[0][0] = 1;

	/* set delta coefficients */
	if (pst->dw.calccoef == 0) {
		for (i = 1; i < pst->dw.num; i++) {
			if (pst->dw.fn[i][0] == ' ') {
				fsize = str2darray(pst->dw.fn[i], &(pst->dw.coef[i]));
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
				pst->dw.coef[i] = dcalloc(fsize, 0);
				freadf(pst->dw.coef[i], sizeof(**(pst->dw.coef)), fsize, fp);
			}

			/* set pointer */
			leng = fsize / 2;
			pst->dw.coef[i] += leng;
			pst->dw.width[i][WLEFT] = -leng;
			pst->dw.width[i][WRIGHT] = leng;
			if (fsize % 2 == 0)
				pst->dw.width[i][WRIGHT]--;
		}
	}
	else if (pst->dw.calccoef == 1) {
		for (i = 1; i < pst->dw.num; i++) {
			leng = atoi(pst->dw.fn[i]);
			if (leng < 1) {
				fprintf(stderr, "Width for regression coefficient shuould be more than 1.\n");
				exit(1);
			}
			pst->dw.width[i][WLEFT] = -leng;
			pst->dw.width[i][WRIGHT] = leng;
			pst->dw.coef[i] = dcalloc(leng*2 + 1, 0);
			pst->dw.coef[i] += leng;
		}

		leng = atoi(pst->dw.fn[1]);
		s2 = 1;
		for (j = 2; j <= leng; j++) {
			x = j * j;
			s2 += x;
		}
		s2 += s2;
		for (j = -leng; j <= leng; j++)
			pst->dw.coef[1][j] = j / s2;

		if (pst->dw.num > 2) {
			leng = atoi(pst->dw.fn[2]);
			s2 = s4 = 1;
			for (j = 2; j <= leng; j++) {
				x = j * j;
				s2 += x;
				s4 += x * x;
			}
			s2 += s2;
			s4 += s4;
			s0 = leng + leng + 1;
			for (j = -leng; j <= leng; j++)
				pst->dw.coef[2][j] = (s0*j*j - s2)/(s4*s0 - s2*s2)/2;
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


double *dcalloc(int x, int xoff)
{
	double		*ptr;

	if ((ptr = (double *) calloc(x, sizeof(*ptr))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	ptr += xoff;
	return(ptr);
}


double **ddcalloc(int x, int y, int xoff, int yoff)
{
	double		*dcalloc(int, int);
	double		**ptr;
	register int	i;

	if ((ptr = (double **) calloc(x, sizeof(*ptr))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	for (i = 0; i < x; i++)
		ptr[i] = dcalloc(y, yoff);
	ptr += xoff;
	return(ptr);
}


double ***dddcalloc(int x, int y, int z, int xoff, int yoff, int zoff)
{
	double		**ddcalloc(int, int, int, int);
	double		***ptr;
	register int	i;

	if ((ptr = (double ***) calloc(x, sizeof(*ptr))) == NULL) {
		fprintf(stderr, "Cannot Allocate Memory\n");
		exit(1);
	}
	for (i = 0; i < x; i++)
		ptr[i] = ddcalloc(y, z, yoff, zoff);
	ptr += xoff;
	return(ptr);
}

int str2darray(char *c, double **x)
{
	int	i, size, sp;
	char	*p, *buf;

	while (isspace(*c))
		c++;
	if (*c == '\0') {
		*x = NULL;
		return(0);
	}

	size = 1;
	sp = 0;
	for (p = c; *p != '\0'; p++) {
		if (!isspace(*p)) {
			if (sp == 1) {
				size++;
				sp = 0;
			}
		}
		else
			sp = 1;
	}
	buf = getmem(strlen(c), sizeof(*buf));
	*x = dgetmem(size);
	for (i = 0; i < size; i++)
		(*x)[i] = strtod(c, &c);
	return(size);
}


/*--------------------------------------------------------------------*/

double *rndpg(PStream *pst)
{
	int		doupdate(PStream *, int);
	void		calc_w(PStream *, int);
	void		calc_alpha(PStream *, int);
	void		calc_k(PStream *, int);
	void		calc_cc(PStream *, int);
	void		update_c(PStream *, int);
	void		update_Q(PStream *, int);
	double		sqrt(double);
	int		tcur, tmin, tmax;
	register int	i, m, u, d;

	pst->sm.t++;
	tcur = pst->sm.t & pst->sm.mask;
	tmin = (pst->sm.t - pst->range) & pst->sm.mask;
	tmax = (pst->sm.t + pst->dw.maxw[WRIGHT]) & pst->sm.mask;

	for (u = -pst->range*2; u <= pst->range*2; u++) {
		for (m = 0; m <= pst->order; m++)
			pst->sm.Q[u][tmax][m] = 0.0;
	}
	for (m = 0; m < pst->vSize; m++) {
		pst->sm.mseq[tmax][m] = pst->mean[m];
		pst->sm.ivseq[tmax][m] = pst->ivar[m];
	}
	for (m = 0; m <= pst->order; m++) {
		if (pst->iType != 2)
			pst->sm.c[tmax][m] = pst->mean[m];
		else
			pst->sm.c[tmax][m] = pst->mean[m] * finv(pst->ivar[m]);
		if (pst->ivar[m] != 0.0) {
			pst->sm.Q[0][tmax][m] = finv(sqrt(pst->ivar[m]));
		}
		else {
			pst->sm.Q[0][tmax][m] = INFTY;
		}
		pst->sm.ivar0[m] = pst->sm.ivseq[tcur][m];
	}

	for (d = 1; d < pst->dw.num; d++) {
		if (doupdate(pst, d)) {
			calc_w(pst, d);
			calc_alpha(pst, d);
			calc_k(pst, d);
			update_c(pst, d);
			update_Q(pst, d);
		}
	}

	for (i = 0; i < pst->nps; i++)
		for (m = 0; m<= pst->order; m++) {
			pst->sm.rnd[tcur][i][m] = bmrnd();
			pst->sm.x[tmin][i][m] = 0.0;
		}
	for (i = 0; i < pst->nps; i++)
		for (m = 0; m<= pst->order; m++) {
			for (u = -pst->range; u <= pst->range; u++)
				if (pst->sm.Q[u][tmin][m] < INFTY2) {
					pst->sm.x[tmin][i][m] += pst->sm.rnd[(tmin+u)&pst->sm.mask][i][m]
								* pst->sm.Q[u][tmin][m];
				}
			pst->sm.x[tmin][i][m] += pst->sm.c[tmin][m];
		}

	if (pst->rnd)
		return(pst->sm.x[tmin][0]);
	else
		return(pst->sm.c[tmin]);
}

int doupdate(PStream *pst, int d)
{
	register int	j;

	if (pst->sm.ivseq[pst->sm.t&pst->sm.mask][(pst->order+1)*d] == 0.0)
		return(0);
	for (j = pst->dw.width[d][WLEFT]; j <= pst->dw.width[d][WRIGHT]; j++)
		if (pst->sm.Q[0][(pst->sm.t+j)&pst->sm.mask][0] == INFTY)
			return(0);
	return(1);
}

void calc_w(PStream *pst, int d)
{
	int	u, m, j;

	for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
		for (m = 0; m <= pst->order; m++) {
			pst->sm.w[u][m] = 0.0;
			for (j = pst->dw.width[d][WLEFT]; j <= pst->dw.width[d][WRIGHT]; j++)
				pst->sm.w[u][m] += pst->sm.Q[u-j][(pst->sm.t+j)&pst->sm.mask][m] * pst->dw.coef[d][j];
		}
}

void calc_alpha(PStream *pst, int d)
{
	int	u, m, n;

	for (m = 0, n = (pst->order+1)*d; m <= pst->order; m++, n++) {
		pst->sm.alpha[m] = 0.0;
		for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
			pst->sm.alpha[m] += pst->sm.w[u][m] * pst->sm.w[u][m];
		pst->sm.alpha[m] *= (pst->sm.ivseq[pst->sm.t&pst->sm.mask][n] - pst->sm.ivar0[n]);
		pst->sm.alpha[m] += 1.0;
	}
}

void calc_k(PStream *pst, int d)
{
	int	u, m, j;

	for (j = -pst->range; j <= pst->dw.maxw[WRIGHT]; j++)
		for (m = 0; m <= pst->order; m++) {
			pst->sm.k[j][m] = 0.0;
			for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
				pst->sm.k[j][m] += pst->sm.w[u][m] * pst->sm.Q[u-j][(pst->sm.t+j)&pst->sm.mask][m];
			pst->sm.k[j][m] /= pst->sm.alpha[m];
		}
}

void update_c(PStream *pst, int d)
{
	int	u, m, n, j;
	double	x;

	for (m = 0, n = (pst->order+1)*d; m <= pst->order; m++, n++) {
		x = 0.0;
		for (j = pst->dw.width[d][WLEFT]; j <= pst->dw.width[d][WRIGHT]; j++)
			x -= pst->dw.coef[d][j] * pst->sm.c[(pst->sm.t+j)&pst->sm.mask][m];
		x *= (pst->sm.ivseq[pst->sm.t&pst->sm.mask][n] - pst->sm.ivar0[n]);
		if (pst->iType != 2)
			x += pst->sm.ivseq[pst->sm.t&pst->sm.mask][n] * pst->sm.mseq[pst->sm.t&pst->sm.mask][n];
		else
			x += pst->sm.mseq[pst->sm.t&pst->sm.mask][n];
		for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
			pst->sm.c[(pst->sm.t+u)&pst->sm.mask][m] += pst->sm.k[u][m] * x;
	}
}

void update_Q(PStream *pst, int d)
{
	int	u, m, n, j;
	double	x;
	double	sqrt(double);

	for (m = 0, n = (pst->order+1)*d; m <= pst->order; m++, n++) {
		x = sqrt(pst->sm.alpha[m]);
		x = x * (pst->sm.ivseq[pst->sm.t&pst->sm.mask][n] - pst->sm.ivar0[n]) / (1.0 + x);
		for (u = -pst->range; u <= pst->dw.maxw[WRIGHT]; u++)
			for (j = -pst->range; j <= pst->dw.maxw[WRIGHT]; j++)
				pst->sm.Q[u-j][(pst->sm.t+j)&pst->sm.mask][m] -= x * pst->sm.w[u][m] * pst->sm.k[j][m];
	}
}


/* Box-Muller, G.E.P.Box and M.E.Muller, Ann. Math. Stat. 29(1958), 610	*/
/*   calculate normally distributed random sequence			*/

#ifndef PI
 	#define	PI	3.14159265358979323846
#endif

double bmrnd(void)
{
	static int	sw = 0;
	static double	t, u;
	double		sqrt(double), log(double), cos(double), sin(double);

	if(sw == 0){
		sw = 1; 
		t = sqrt(-2.0 * log(1.0 - drand48()));
		u = 2.0 * PI * drand48();
		return t * cos(u);
	}
	else{
		sw = 0;
		return t * sin(u);
	}
}

