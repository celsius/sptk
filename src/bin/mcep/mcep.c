/************************************************************************
*									*
*    Mel Cepstral Analysis						*
*									*
*					1993.5  K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		mcep [ options ] [infile] > stdout			*
*	options:							*
*		-a alpha :  all-pass constant		     [0.35]	*
*		-m m     :  order of mel cepstrum	     [25]	*
*		-l l     :  frame length		     [256]	*
*		(level 2)						*
*		-i i     :  minimum iteration		     [2]	*
*		-j j     :  maximum iteration		     [30]	*
*		-d d     :  end condition		     [0.001]	*
*		-e e     :  small value added to periodgram [0.0]	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(L-1),				*
*	stdout:								*
*		mel cepstrum						*
*		    , c~(0), c~(1), ..., c~(M),				*
*	require:							*
*		mcep()							*
*									*
************************************************************************/

static char *rcs_id = "$Id: mcep.c,v 1.1 2000/03/01 13:58:41 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
int	mcep();


/*  Default Values  */
#define	ALPHA		0.35
#define ORDER		25
#define FLENG           256
#define MINITR          2
#define MAXITR          30
#define END             0.001
#define EPS             0.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - mel cepstral analysis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -a a  : all-pass constant               [%g]\n", ALPHA);
    fprintf(stderr, "       -m m  : order of mel cepstrum           [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : frame length                    [%d]\n", FLENG);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "     (level 2)\n");
    fprintf(stderr, "       -i i  : minimum iteration               [%d]\n", MINITR);
    fprintf(stderr, "       -j j  : maximum iteration               [%d]\n", MAXITR);
    fprintf(stderr, "       -d d  : end condition                   [%g]\n", END);
    fprintf(stderr, "       -e e  : small value added to periodgram [%g]\n",EPS);
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequences (float)              [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       mel-cepstrum (float)\n");
    fprintf(stderr, "\n");

    exit(status);
}


void main(int argc, char **argv)
{
    int	     m = ORDER, flng = FLENG, itr1 = MINITR, itr2 = MAXITR, flag = 0;
    FILE     *fp = stdin;
    double   *mc, *x, a = ALPHA, end = END, e = EPS, atof();    

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
	        case 'a':
		    a = atof(*++argv);
		    --argc;
		    break;
		case 'm':
		    m = atoi(*++argv);
		    --argc;
		    break;
		case 'l':
		    flng = atoi(*++argv);
		    --argc;
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
    mc = x + flng;
    
    while (freadf(x, sizeof(*x), flng, fp) == flng){
	flag = mcep(x, flng, mc, m, a, itr1, itr2, end, e);
	fwritef(mc, sizeof(*mc), m+1, stdout);
    }
    exit(0);
}

