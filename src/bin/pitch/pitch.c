/************************************************************************
*									*
*    Pitch Extract							*
*									*
*					1998.7  M.Tamura		*
*									*
*	usage:								*
*		pitch [ options ] [ infile ] > stdout			*
*	options:							*
*		-m  m    :  order of sequence				*
*		-l  l    :  FFT size					*
*		-a  a	 :  pre-emphasis coefficient			*
*		-L  L	 :  minimum for pitch period			*
*		-H  H	 :  Maximum for pitch period			*
*		-e  e	 :  small value for calculate log		*
*		-n  n	 :  order for calculate log-spectral envelope	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		pitch period						*
*		    p(t)						*
*	require:							*
*		pitch()							*
*									*
************************************************************************/

static char rcsid[] = "$Id: pitch.c,v 1.1 2000/03/01 13:58:49 yossie Exp $";

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
double	pitch();

/*  Default Values  */
#define FLNG		512
#define ORDER		399
#define PRE		0.6
#define EPS		0.0
#define LOW		50
#define HIGH		150
#define THRESH		8.2
#define ENV		25

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
    fprintf(stderr, "       -m m  : order of sequence             [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : FFT size                      [%d]\n", FLNG);
    fprintf(stderr, "       -t t  : voiced/unvoiced threshold     [%g]\n", THRESH);
    fprintf(stderr, "       -a a  : pre-emphasis coefficient      [%g]\n", PRE);
    fprintf(stderr, "       -L L  : minimum of pitch period       [%d]\n", LOW);
    fprintf(stderr, "       -H H  : maximum of pitch period       [%d]\n", HIGH);
    fprintf(stderr, "       -e e  : small value for calculate log [%g]\n", EPS);
    fprintf(stderr, "       -n n  : cepstrum order for calculate  [%d]\n",ENV);
    fprintf(stderr, "               log-spectral envelope\n");
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
    int		m = ORDER, l = FLNG, L = LOW , H = HIGH, env = ENV;
    double	*x, eps = EPS, a = PRE, atof(),
		max, p, thresh = THRESH;
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
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 't':
		    thresh = atof(*++argv);
		    --argc;
		    break;
		case 'e':
		    eps = atof(*++argv);
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
		case 'n':
		    env = atoi(*++argv);
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

    while (freadf(x, sizeof(*x), m+1, fp) == m+1){
	fillz(x+m+1,l-m+1,sizeof(double));
	p = pitch(x, m, l, a, thresh, L, H, eps, env);
	fwritef(&p, sizeof(p), 1, stdout);
    }
    exit(0);
}
