/************************************************************************
*									*
*    LPC Analysis Using Levinson-Durbin	Method				*
*									*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		lpc [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l     :  frame length		[256]		*
*		-m m     :  order of LPC		[25]		*
*	infile:								*
*		data sequence 						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		LP Coefficeints						*
*		    , K, a(1), ..., a(m),				*
*	require:							*
*		lpc()							*
*									*
************************************************************************/

static char *rcs_id = "$Id: lpc.c,v 1.1 2000/03/01 13:58:39 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
int	lpc();


/*  Default Values  */
#define ORDER		25
#define FLNG		256


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - LPC analysis using Levinson-Durbin method\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : frame length       [%d]\n", FLNG);
    fprintf(stderr, "       -m m  : order of LPC       [%d]\n", ORDER);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequence (float)  [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       LP coefficients (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, l = FLNG, flag;
    FILE	*fp = stdin;
    double	*x, *a;
    
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
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    x = dgetmem(l+m+1);
    a = x + l;

    while (freadf(x, sizeof(*x), l, fp) == l){
	flag = lpc(x, l, a, m);
	fwritef(a, sizeof(*a), m+1, stdout);
    }
    exit(0);
}








