/************************************************************************
*									*
*    Transform Real Sequence to Spectrum				*
*									*
*					1985.11  K.Tokuda		*
*					1996. 5  K.Koishida		*
*									*
*	usage:								*
*		spec [options] [infile] > stdout			*
*	options:							*
*		-l l     :  frame length			[256]	*
*		-m m     :  order of MA part			[l-1]	*
*		-n n     :  order of AR part			[0]	*
*		-z z     :  MA coefficients filename		[NULL]	*
*		-p p     :  AR coefficients filename		[NULL]	*
*		-e e     :  small value for calculating log()	[0]	*
*		-o o     :  output format (see stdout)		[0]	*
*	infile:								*
*		real sequence						*
*		    , c(0), c(1), ..., c(l-1),				*
*	stdout:								*
*	        output format       scale				*
*		       0 :	    20 * log|H(z)|			*
*		       1 :          ln|H(z)|				*
*		       2 :          |H(z)|				*
*		       3 :          Re[H(z)], Im[H(z)] 
*		spectrum						*
*		    , s(0), s(1), ..., s(L/2),				*
*									*
************************************************************************/

static char *rcs_id = "$Id: spec.c,v 1.1.1.1 2000/03/01 13:58:50 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
double  log(), log10(), sqrt();


/*  Default Values  */
#define LENG		256
#define ORDERMA		0
#define ORDERAR		0
#define EPS		0.0
#define OTYPE		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform real sequence to spectrum\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : frame length                      [%d]\n", LENG);
    fprintf(stderr, "       -m m  : order of MA part                  [%d]\n", ORDERMA);
    fprintf(stderr, "       -n n  : order of AR part                  [%d]\n", ORDERAR);
    fprintf(stderr, "       -z z  : MA coefficients filename          [NULL]\n");
    fprintf(stderr, "       -p p  : AR coefficients filename          [NULL]\n");
    fprintf(stderr, "       -e e  : small value for calculating log() [%g]\n", EPS);
    fprintf(stderr, "       -o o  : output format                     [%d]\n", OTYPE);
    fprintf(stderr, "                 0 (20 * log|H(z)|)\n");
    fprintf(stderr, "                 1 (ln|H(z)|)\n");
    fprintf(stderr, "                 2 (|H(z)|)\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       real sequences (float)                    [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       spectrum (float)\n");
    fprintf(stderr, "\n");

    exit(status);
}

void main(int argc, char **argv)
{
    int     leng = LENG, otype = OTYPE, orderma = ORDERMA, 
    	    orderar = ORDERAR, no, i;
    char    *filema = "", *filear = "";
    FILE    *fp = stdin, *fpma = NULL, *fpar = NULL;
    double  eps = EPS, k, *x, *y, *mag, atof();

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    leng = atoi(*++argv);
		    --argc;
		    break;
		case 'm':
		    orderma = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    orderar = atoi(*++argv);
		    --argc;
		    break;
		case 'z':
		    filema = *++argv;
		    --argc;
		    break;
		case 'p':
		    filear = *++argv;
		    --argc;
		    break;
		case 'e':
		    eps = atof(*++argv);
		    --argc;
		    break;
		case 'o':
		    otype = atoi(*++argv);
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
	    fp = getfp(*argv, "r");
    
    no = leng / 2 + 1;

    x = dgetmem(leng+leng+no);
    y = x + leng;
    mag = y + leng;

    if(*filema == '\0' && *filear == '\0'){
	if(orderar > 0)
	    fpar = fp;
	else
	    fpma = fp;
	
	if(orderma <= 0)
	    orderma = leng - 1;
    }
    
    if(*filema != '\0')
	fpma = getfp(filema, "r");
    if(*filear != '\0')
	fpar = getfp(filear, "r");

    for(;;){
	if(fpma == NULL && fpar != NULL)
	    for(i=0; i<no; i++) mag[i] = 1.0;
	else{
	    fillz(x, sizeof(*x), leng);
	    if(freadf(x, sizeof(*x), orderma+1, fpma) == 0)
		exit(0);

	    fftr(x, y, leng);
	    for(i=0; i<no; i++)
		mag[i] = x[i] * x[i] + y[i] * y[i];
	}

	if(fpar != NULL){
	    fillz(x, sizeof(*x), leng);
	    if(freadf(x, sizeof(*x), orderar+1, fpar) == 0)
		exit(0);
	    k = x[0] * x[0];
	    x[0] = 1.0;
	    fftr(x, y, leng);
	    for(i=0; i<no; i++)
		mag[i] *= k / (x[i] * x[i] + y[i] * y[i]);
	}

	switch (otype){
	    case 1 :
		for(i=0; i<no; i++)
		    x[i] = 0.5 * log(mag[i] + eps);
		fwritef(x, sizeof(*x), no, stdout);
		break;
	    case 2 :
		for(i=0; i<no; i++)
		    x[i] = sqrt(mag[i]);
		fwritef(x, sizeof(*x), no, stdout);
		break;
	    default:
		for(i=0; i<no; i++)
		    x[i] = 10 * log10(mag[i] + eps);
		fwritef(x, sizeof(*x), no, stdout);
		break;
	}
    }
    exit(0);
}
