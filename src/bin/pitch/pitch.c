/**************************************************************************
*									  *
*    Pitch Extract							  *
*									  *
*					1998.7  M.Tamura		  *
*                                       2000.3  T.Tanaka                  *
*									  *
*	usage:								  *
*		pitch [ options ] [ infile ] > stdout			  *
*	options:							  *
*               -s  s      :  sampling frequency                          *
*		-n  n      :  order of sequence				  *
*		-l  l      :  FFT size					  *
*		-a  a	   :  pre-emphasis coefficient			  *
*               -t  thresh :  voiced/unvoiced threshhold                  *
*		-L  L	   :  minmum fundamental frequency to search for  *
*		-H  H	   :  maximum fundamental frequency to search for *
*		-e  eps	   :  small value for calculate log		  *
*               (level 2 :  for LPC mel cepstral analysis)                *
*               -m  m1      :  order of LPC                               *
*               -M  m2      :  order of mel cepstrum                      *
*               -A  alpha  :  all-pass constant                           *
*	infile:								  *
*		data sequence						  *
*		    , x(0), x(1), ..., x(n-1),				  *
*	stdout:								  *
*		pitch   						  *
*		    p(t)						  *
*	require:							  *
*		pitch()							  *
*									  *
**************************************************************************/

static char rcsid[] = "$Id$";

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
double	pitch();

/*  Default Values  */
#define FREQ            10
#define ILNG		400
#define FLNG		512
#define THRESH		6.0
#define LOW		60
#define HIGH		240
#define PRE		0.95
#define EPS		0.0

/*  Default Values for LPC mel cepstrum */
#define	ALPHA		0.35
#define LPC		25
#define MC              25 

/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - pitch extract\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -s s  : sampling frequency [kHz]        [%d]\n", FREQ);
    fprintf(stderr, "       -n n  : order of sequence               [%d]\n", ILNG);
    fprintf(stderr, "       -l l  : FFT size                        [%d]\n", FLNG);
    fprintf(stderr, "       -t t  : voiced/unvoiced threshold       [%.1f]\n", THRESH);
    fprintf(stderr, "       -a a  : pre-emphasis coefficient        [%g]\n", PRE);
    fprintf(stderr, "       -L L  : minimum fundamental             [%d]\n", LOW);
    fprintf(stderr, "               frequency to serach for\n");
    fprintf(stderr, "       -H H  : maximum fundamental             [%d]\n", HIGH);
    fprintf(stderr, "               frequency to serach for\n");
    fprintf(stderr, "       -e e  : small value for calculate       [%g]\n", EPS);
    fprintf(stderr, "               log-spectral envelope\n");
    fprintf(stderr, "     (level 2 : for LPC mel cepstral analysis)\n");
    fprintf(stderr, "       -m m  : order of LPC                    [%d]\n", LPC);
    fprintf(stderr, "       -M M  : order of mel cepstrum           [%d]\n", LPC);
    fprintf(stderr, "       -A A  : all-pass constant               [%g]\n", ALPHA);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequence (float)             [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       pitch (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		i, freq = FREQ, n = ILNG, l = FLNG, L = LOW , H = HIGH, m1 = LPC, m2 = MC;
    double	*x, eps = EPS, a = PRE, atof(),	p, thresh = THRESH, alpha = ALPHA;
    FILE	*fp = stdin;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 's':
		    freq = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    n = atoi(*++argv);
		    --argc;
		    break;
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 't':
		    thresh = atof(*++argv);
		    --argc;
		    break;
		case 'a':
		    a = atof(*++argv);
		    --argc;
		    break;
		case 'L':
		    L = atoi(*++argv);
		    --argc;
		    break;
		case 'H':
		    H = atoi(*++argv);
		    --argc;
		    break;
		case 'e':
		    eps = atof(*++argv);
		    --argc;
		    break;
	        case 'm':
		    m1 = atoi(*++argv);
		    --argc;
		    break;
	        case 'M':
		    m2 = atoi(*++argv);
		    --argc;
		    break;
	        case 'A':
		    alpha = atof(*++argv);
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

    x = dgetmem(l);
    while (freadf(x, sizeof(*x), n, fp) == n){
	fillz(x+n,l-n,sizeof(double));
	p = pitch(x, freq, l, a, thresh, L, H, eps, m1, m2, alpha);
	fwritef(&p, sizeof(p), 1, stdout);
    }
    exit(0);
}
