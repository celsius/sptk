/************************************************************************
*									*
*    Pitch Extract							*
*									*
*					1998.7  M.Tamura		*
*                                       2000.3  T.Tanaka                *
*									*
*	usage:								*
*		pitch [ options ] [ infile ] > stdout			*
*	options:							*
*               -s  s     :  sampling frequency			[10]	*
*		-l  l     :  frame length 			[400]	*
*		-a  a	  :  pre-emphasis coefficient		[0.95]	*
*               -t  t     :  voiced/unvoiced threshhold		[6.0]	*
*		-L  L	  :  minmum fundamental frequency	[60]	*
*			     to search for (Hz)				*
*		-H  H	  :  maximum fundamental frequency	[240]	*
*		  	     to search for (Hz)				*
*		-e  e	  :  small value for calculate log	[0.0]	*
*               (level 2 :  for uels cepstral analysis)                 *
*               -i  i     :  minimum number of iteration	[2]	*
*               -j  j     :  maximum number of iteration	[30]	*
*               -d  d     :  end condition			[0.1]	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(n-1),				*
*	stdout:								*
*		pitch   						*
*		    p(t)						*
*	require:							*
*		pitch()							*
*									*
*************************************************************************/

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
#define THRESH		6.0
#define LOW		60
#define HIGH		240
#define PRE		0.95
#define EPS		0.0

/*  Default Values for uels */
#define	MINITR		2
#define MAXITR		30
#define END		0.1

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
    fprintf(stderr, "       -s s  : sampling frequency (kHz)        [%d]\n", FREQ);
    fprintf(stderr, "       -l l  : frame length                    [%d]\n", ILNG);
    fprintf(stderr, "       -t t  : voiced/unvoiced threshold       [%.1f]\n", THRESH);
    fprintf(stderr, "       -a a  : pre-emphasis coefficient        [%g]\n", PRE);
    fprintf(stderr, "       -L L  : minimum fundamental             [%d]\n", LOW);
    fprintf(stderr, "               frequency to serach for (Hz)\n");
    fprintf(stderr, "       -H H  : maximum fundamental             [%d]\n", HIGH);
    fprintf(stderr, "               frequency to serach for (Hz)\n");
    fprintf(stderr, "       -e e  : small value for calculate       [%g]\n", EPS);
    fprintf(stderr, "               log-spectral envelope\n");
    fprintf(stderr, "     (level 2 : for uels cepstral analysis)\n");
    fprintf(stderr, "       -i i  : minimum number of iteration     [%d]\n", MINITR);
    fprintf(stderr, "       -j j  : maximum number of iteration     [%d]\n", MAXITR);
    fprintf(stderr, "       -d d  : end condition                   [%g]\n", END);
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
    int		i, freq = FREQ, n = ILNG, l, L = LOW , H = HIGH, m, itr1 = MINITR, itr2 = MAXITR, low, high;
    double	*x, eps = EPS, a = PRE, atof(),	p, thresh = THRESH, end = END;
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
		case 'l':
		    n = atoi(*++argv);
		    --argc;
		    break;
		case 'a':
		    a = atof(*++argv);
		    --argc;
		    break;
		case 't':
		    thresh = atof(*++argv);
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
		case 'i':
		    itr1 = atoi(*++argv);
		    --argc;
		case 'j':
		    itr2 = atoi(*++argv);
		    --argc;
		case 'd':
		    end = atof(*++argv);
		    --argc;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    low = freq * 1000 / H;
    high = freq * 1000 / L;
    m = freq / 10 * 25;
    l = 1;
    while (l < n)l+=l;

    x = dgetmem(l);

    while (freadf(x, sizeof(*x), n, fp) == n){
	fillz(x+n,l-n,sizeof(double));
	p = pitch(x, l, a, thresh, low, high, eps, m, itr1, itr2, end);
	fwritef(&p, sizeof(p), 1, stdout);
    }
    exit(0);
}
