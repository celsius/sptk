/************************************************************************
*									*
*    Transform Mel-Generalize Cepstrum to Spectrum			*
*									*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		mgc2sp [ options ] [ infile ] > stdout			*
*	options:							*
*		-m  m    :  order of mel-genralized cepstrum   	[25]	*
*		-a  a    :  alpha 				[0.0] 	*
*		-g  g    :  gamma 				[0.0] 	*
*		-n       :  regard input as normalized cepstrum [FALSE] *
*		-u       :  regard input as multiplied by gamma [FALSE] *
*		-l  l    :  FFT length 				[256] 	*
*		-o  o    :  output format (see stdout)		[0] 	*
*		-p       :  output phase			[FALSE]	*
*	infile:								*
*		mel-generalized cepstrum				*
*		    , c(0), c(1), ..., c(m),				*
*	stdout:								*
*	        output format       scale				*
*		       0 :	      20 * log|H(z)|			*
*		       1 :            ln|H(z)|				*
*		       2 :            |H(z)|				*
*	        (-p option is used)					*
*		       0 :	      arg|H(z)| / pi       [pi rad]	*
*		       1 :            arg|H(z)|            [rad]	*
*		       2 :            arg|H(z)| * 180 / pi [deg]	*
*		spectrum						*
*		    , s(0), s(1), ..., s(L/2),				*
*	notice:								*
*		if g > 1.0, g = -1 / g .				*
*	require:							*
*		mgc2sp()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: mgc2sp.c,v 1.1.1.1 2000/03/01 13:58:42 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
void	mgc2sp();
double  exp(), log();


/*  Default Values  */
#define ORDER		25
#define ALPHA		0.0
#define GAMMA		0.0
#define NORM		FA
#define LENG		256
#define OTYPE		0
#define PHASE		FA
#define MULG		FA 


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform mel-generalized cepstrum to spectrum\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -a a  : alpha                               [%g]\n", ALPHA);
    fprintf(stderr, "       -g g  : gamma                               [%g]\n", GAMMA);
    fprintf(stderr, "       -m m  : order of mel-genralized cepstrum    [%d]\n", ORDER);
    fprintf(stderr, "       -n    : regard input as normalized cepstrum [%s]\n", BOOL[NORM]);
    fprintf(stderr, "       -u    : regard input as multiplied by gamma [%s]\n", BOOL[MULG]);
    fprintf(stderr, "       -l l  : FFT length                          [%d]\n", LENG);
    fprintf(stderr, "       -p    : output phase                        [%s]\n", BOOL[PHASE]);
    fprintf(stderr, "       -o o  : output format                       [%d]\n", OTYPE);
    fprintf(stderr, "                 0 (20*log|H(z)|)\n");
    fprintf(stderr, "                 1 (ln|H(z)|)\n");
    fprintf(stderr, "                 2 (|H(z)|)\n");
    fprintf(stderr, "             -p option is specified\n");
    fprintf(stderr, "                 0 (arg|H(z)|/pi     [pi rad])\n");
    fprintf(stderr, "                 1 (arg|H(z)|        [rad])\n");
    fprintf(stderr, "                 2 (arg|H(z)|*180/pi [deg])\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       mel-generalized cepstrum (float)            [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       spectrum (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if g > 1.0, g = -1 / g .\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, l = LENG, otype = OTYPE, no, i;
    double	alpha = ALPHA, gamma = GAMMA, *c, *x, *y, logk, atof();
    Boolean	norm = NORM, phase = PHASE, mulg = MULG;
    FILE	*fp = stdin;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'm':
		    m = atoi(*++argv);
		    --argc;
		    break;
		case 'a':
		    alpha = atof(*++argv);
		    --argc;
		    break;
		case 'g':
		    gamma = atof(*++argv);
		    --argc;
		    if (gamma > 1.0) gamma = -1.0 / gamma;
		    break;
		case 'n':
		    norm = 1 - norm;
		    break;
		case 'u':
		    mulg = 1 - mulg;
		    break;
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'o':
		    otype = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    phase = 1 - phase;
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

    x = dgetmem(l+l+m+1);
    y = x + l;
    c = y + l;
    
    no = l / 2 + 1;
    logk = 20.0 / log(10.0);
    
    while (freadf(c, sizeof(*c), m+1, fp) == m+1){
	if(norm) 
	    ignorm(c, c, m, gamma);
	else if(mulg)
	    c[0] = (c[0] - 1.0) / gamma;

	if(mulg)
	    for(i=m;i>0;i--) 
		c[i] /= gamma;

	mgc2sp(c, m, alpha, gamma, x, y, l);

	if (phase)
	    switch (otype){
	        case 1 :
		    for (i = no; i--;) x[i] = y[i];
		    break;
		case 2 :
		    for (i = no; i--;)
			x[i] = y[i] * 180 / PI;
		    break;
		default :		
		    for (i = no; i--;)
			x[i] = y[i] / PI;
		    break;
		}
	else
	    switch (otype){
	        case 1 :
		    break;
                case 2 :
		    for (i = no; i--;)
			x[i] = exp(x[i]);
	            break;
		default :
		    for (i = no; i--;)
			x[i] *= logk;
	            break;
		}

	fwritef(x, sizeof(*x), no, stdout);
    }
    exit(0);
}
