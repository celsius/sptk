/************************************************************************
*									*
*    Interpolation							*
*									*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		interpolate [ options ] [ infile ] > stdout		*
*	options:							*
*		-p p     :  interpolation period	[10]		*
*		-s s     :  start sample		[0]		*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ...					*
*	stdout:								*
*		decimated data						*
*		    , 0, ..., 0, x(0), 0, ..., x(1), 0, ...,		*
*		      ---------	 ------------- -------------		*
*		         s-1           p            p			*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define PERIOD		10
#define START		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - interpolation \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -p p  : interpolation period [%d]\n", PERIOD);
    fprintf(stderr, "       -s s  : start sample         [%d]\n", START);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)        [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       interpolated data sequence (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		period = PERIOD, start = START, i;
    FILE	*fp = stdin;
    double	*x;
    
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'p':
		    period = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    start = atoi(*++argv);
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

    x = dgetmem(period);
    
    fwritef(x, sizeof(*x), start, stdout);

    while(freadf(x, sizeof(*x), 1, fp) == 1)
	fwritef(x, sizeof(*x), period, stdout);

    exit(0);
}

