/************************************************************************
*									*
*    Mel-Generalized Cepstral Analysis for gamma=-1/2			*
*									*
*					1990.7  K.Tokuda		*
*					1996.3  K.Koishida		*
*					1998.10 K.Koishida		*
*									*
*	usage:								*
*		mgcep2 [ options ] [ infile ] > stdout			*
*	options:							*
*		-a a     :  alapha				[0.35]	*
*		-m m     :  order of mel-generalized cepstrum	[25]	*
*		-l l     :  frame length			[256]	*
*		-o o     :  output format  (see stdout)		[0]	*
*		(level 2)						*
*		-i i     :  minimum iteration			[2]	*
*		-j j     :  maximum iteration			[30]	*
*		-d d     :  end condition			[0.001]	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(L-1),				*
*	stdout:								*
*	      output format	coefficients				*
*		    0		, c~(0), c~(1), ..., c~(m)		*
*		    1		, b(0), b(1), ..., b(m)			*
*		    2		, K~, c~'(1), ..., c~'(m)		*
*		    3		, K, b'(1), ..., b'(m)			*
*		    4		, K~, g*c~'(1), ..., g*c~'(m)		*
*		    5		, K, g*b'(1), ..., g*b'(m)		*
*       notice:                                                         *
*               if g > 1.0, g = -1 / g .                                *
*	require:							*
*		mgcep()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
int	mgcep2();


/*  Default Values  */
#define	ALPHA		0.35
#define ORDER		25
#define FLENG           256
#define OTYPE           0
#define MINITR          2
#define MAXITR          30
#define END             0.001


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - mel-generalized cepstral analysis for gamma=-1/2\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -a a  : alpha                             [%g]\n", ALPHA);
    fprintf(stderr, "       -m m  : order of mel-generalized cepstrum [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : frame length                      [%d]\n", FLENG);
    fprintf(stderr, "       -o o  : output format                     [%d]\n", OTYPE);
    fprintf(stderr, "                 0 (c~0...c~m)\n");
    fprintf(stderr, "                 1 (b0...bm)\n");
    fprintf(stderr, "                 2 (K~,c~'1...c~'m)\n");
    fprintf(stderr, "                 3 (K,b'1...b'm)\n");
    fprintf(stderr, "                 4 (K~,g*c~'1...g*c~'m)\n");
    fprintf(stderr, "                 5 (K,g*b'1...g*b'm)\n");
    fprintf(stderr, "     (level 2)\n");
    fprintf(stderr, "       -i i  : minimum iteration                 [%d]\n", MINITR);
    fprintf(stderr, "       -j j  : maximum iteration                 [%d]\n", MAXITR);
    fprintf(stderr, "       -d d  : end condition                     [%g]\n", END);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequence (float)                 [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       mel-generalized cepstrum (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int	     m = ORDER, flng = FLENG, itr1 = MINITR, itr2 = MAXITR, 
	     flag = 0, otype = OTYPE, i;
    FILE     *fp = stdin;
    double   *c, *x, a = ALPHA, end = END, g = -0.5, atof();    
    
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
		case 'o':
		    otype = atoi(*++argv);
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
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    x = dgetmem(flng+m+m+2);
    c = x + flng;
    
    while (freadf(x, sizeof(*x), flng, fp) == flng){
	flag = mgcep2(x, flng, c, m, a, itr1, itr2, end);

	if(otype == 1 || otype == 3 || otype == 5)
	    if(a != 0.0) mc2b(c, c, m, a);      /* c~ --> b */

	if(otype == 2 || otype == 3 || otype == 4 || otype == 5)
	    gnorm(c, c, m, g);               /* c~ --> c~' or b --> b' */

	if(otype == 4 || otype == 5)
	    for (i = m; i >= 1; i--) c[i] *= g;

	fwritef(c, sizeof(*c), m+1, stdout);
    }
    exit(0);
}

