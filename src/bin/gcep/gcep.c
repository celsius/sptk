/************************************************************************
*									*
*    Generalized Cepstral Analysis					*
*									*
*					1988.11 K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		gcep [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of generalized cepstrum    [25]	*
*		-g g     :  gamma		             [0]	*
*		-l l     :  frame length	             [256]	*
*		-n       :  output normalized cepstrum	     [FALSE]	*
*		(level2)						*
*		-i i     :  minimum iteration		     [3]	*
*		-j j     :  maximum iteration		     [10]	*
*		-d d     :  end condition		     [0.001]	*
*		-e e     :  small value added to periodgram  [0.0]	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(L-1),				*
*	stdout:								*
*		generalized cepstral coefficeints			*
*		    , c(0), c(1), ..., c(M),				*
*       notice:                                                         *
*               if g > 1.0, g = -1 / g .                                *
*	require:							*
*		gcep()							*
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
int	gcep();


/*  Default Values  */
#define ORDER		25
#define GAMMA		0.0
#define	FLENG		256
#define	NORM		FA
#define	MINITR		2
#define	MAXITR		30
#define	END		0.001
#define EPS		0.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - generalized cepstral analysis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of generalized cepstrum    [%d]\n", ORDER);
    fprintf(stderr, "       -g g  : gamma                            [%d]\n", GAMMA);
    fprintf(stderr, "       -l l  : frame length                     [%d]\n", FLENG);
    fprintf(stderr, "       -n    : output normalized cepstrum       [%s]\n", BOOL[NORM]);
    fprintf(stderr, "     (level2)\n");
    fprintf(stderr, "       -i i  : minimum iteration                [%d]\n", MINITR);
    fprintf(stderr, "       -j j  : maximum iteration                [%d]\n", MAXITR);
    fprintf(stderr, "       -d d  : end condition                    [%g]\n", END);
    fprintf(stderr, "       -e e  : small value added to periodgram  [%g]\n", EPS);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequence (float)          [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       generalized cepstrum (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if g > 1.0, g = -1 / g\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, flng = FLENG, itr1 = MINITR, 
		itr2 = MAXITR, norm = NORM, flag = 0;
    FILE	*fp = stdin;
    double	*gc, *x, g = GAMMA, end = END, e = EPS, atof();
    
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
		    flng = atoi(*++argv);
		    --argc;
		    break;
		case 'g':
		    g = atof(*++argv);
		    --argc;
                    if (g > 1.0) g = -1.0 / g;
		    break;
		case 'n':
		    norm = 1 - norm;
		    break;
		case 'i':
		    itr1 = atoi(*++argv);
		    --argc;
		    break;
		case 'j':
		    itr2 = atoi(*++argv);
		    --argc;
		    break;
		case 'd':
		    end = atof(*++argv);
		    --argc;
		    break;
		case 'e':
		    e = atof(*++argv);
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

    x = dgetmem(flng+m+1);
    gc = x + flng;

    while (freadf(x, sizeof(*x), flng, fp) == flng){
	flag = gcep(x, flng, gc, m, g, itr1, itr2, end, e);
	if(! norm)
	    ignorm(gc, gc, m, g);
	fwritef(gc, sizeof(*gc), m+1, stdout);
    }
    exit(0);
}
