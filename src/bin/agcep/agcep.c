/************************************************************************
*									*
*    Adaptive Generalized Cepstral Analysis				*
*									*
*					1991.11 K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		agcep [ options ] [ pefile ] < stdin > stdout		*
*	options:							*
*		-m m     :  order of generalized cepstrum	[25]	*
*		-g g     :  -1 / gamma				[1]	*
*		-l l     :  leakage factor			[0.98]	*
*		-t t     :  momentum constant			[0.9]	*
*		-k k     :  step size				[0.1]	*
*		-p p     :  output period of cepstrum		[1]	*
*		-s       :  smooth (average) cepstrum		[FALSE]	*
*		-n       :  output normalized cepstrum 		[FALSE]	*
*		-e e	 :  minimum value for epsilon		[0.0]	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ...	 				*
*	stdout:								*
*		generalized cepstrum					*
*		    , c(0), c(1), ..., c(m),				*
*	output:								*
*		prediction error (if pefile is specified)		*
*		    , e(0), e(1), ...					*
*	require:							*
*		iglsadf1(), ignorm()					*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
double	iglsadf1(), sqrt();


/*  Default Values  */
#define ORDER		25
#define STAGE		1
#define LAMBDA		0.98
#define STEP		0.1
#define PERIOD		1
#define AVERAGE		FA
#define NORM		FA
#define TAU		0.9
#define EPS		0.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - adaptive generalized cepstral analysis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ pefile ] < stdin > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of generalized cepstrum          [%d]\n", ORDER);
    fprintf(stderr, "       -g g  : -1 / gamma                             [%d]\n", STAGE);
    fprintf(stderr, "       -l l  : leakage factor                         [%g]\n", LAMBDA);
    fprintf(stderr, "       -t t  : momentum constant                      [%g]\n", TAU);
    fprintf(stderr, "       -k k  : step size                              [%g]\n", STEP);
    fprintf(stderr, "       -p p  : output period of generalized cepstrum  [%d]\n", PERIOD);
    fprintf(stderr, "       -s    : output smoothed generalized cepstrum   [%s]\n", BOOL[AVERAGE]);
    fprintf(stderr, "       -n    : output normalized generalized cepstrum [%s]\n", BOOL[NORM]);
    fprintf(stderr, "       -e e  : minimum value for epsilon              [%g]\n", EPS);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  stdin:\n");
    fprintf(stderr, "       data sequence (float)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       generalized cepstrum (float)\n");
    fprintf(stderr, "  pefile:\n");
    fprintf(stderr, "       prediction error (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int	     m = ORDER, period = PERIOD, stage = STAGE, i, j;
    FILE     *fp = stdin, *fpe = NULL;
    Boolean  ave = AVERAGE, norm = NORM;
    double   lambda = LAMBDA, step = STEP,eps = EPS,
             *c, *cc, *eg, *ep, *d, *avec, tau = TAU,
             x, ee, ll, gg, mu, gamma, tt, ttx,atof();

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
	        case 'l':
		    lambda = atof(*++argv);
		    --argc;
		    break;
		case 't':
		    tau = atof(*++argv);
		    --argc;
		    break;
	        case 'k':
		    step = atof(*++argv);
		    --argc;
		    break;
		case 'm':
		    m = atoi(*++argv);
		    --argc;
		    break;
		case 'g':
		    stage = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    period = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    ave = 1 - ave;
		    break;
		case 'n':
		    norm = 1 - norm;
		    break;
		case 'e':
		    eps = atof(*++argv);
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
	    fpe = getfp(*argv, "w");

    c  = dgetmem(5*(m+1)+m*stage);
    cc = c  + m + 1;
    eg = cc + m + 1;
    ep = eg + m + 1;
    avec = ep + m + 1;
    d = avec  + m + 1;

    gamma = -1.0 / (double)stage;
    
    j  = period;
    ll = 1.0 - lambda;
    gg = 1.0;
    ee = 1.0;
    step /= (double)m;
    tt = 2 * (1.0 - tau);
    
    while (freadf(&x, sizeof(x), 1, fp) == 1){
	eg[m] = d[stage*m-1];
	x = iglsadf1(x, c, m, stage, d);

	movem(d+(stage-1)*m, eg, sizeof(*d), m);
	
	gg = lambda * gg  + ll * eg[0] * eg[0];
	gg = ( gg < eps )? eps : gg;
	mu = step / gg;
	ttx = tt * x;
	
	for(i=1; i<=m; i++){
	    ep[i] = tau * ep[i] - ttx * eg[i];
	    c[i] -= mu * ep[i];
	}

	ee = lambda * ee + ll * x * x;
	c[0] = sqrt(ee);

	if(ave)
	    for(i=0; i<=m; i++)
		avec[i] += c[i];

	if (fpe != NULL)
	    fwritef(&x, sizeof(x), 1, fpe);
	
	if (--j == 0){
	    j = period;
	    if (ave){
		for(i=0; i<=m; i++)
		    avec[i] /= period;
		if(! norm)
		    ignorm(avec, cc, m, gamma);
		fwritef(cc, sizeof(*cc), m+1, stdout);
		fillz(avec, sizeof(*avec), m+1);
	    }
	    else if (! norm){
		ignorm(c, cc, m, gamma);
		fwritef(cc, sizeof(*cc), m+1, stdout);
	    }
	    else
		fwritef(c, sizeof(*c), m+1, stdout);
	}
    }
    exit(0);
}

