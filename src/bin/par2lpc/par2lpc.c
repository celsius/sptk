/************************************************************************
*									*
*    Transform PARCOR to LPC						*
*									*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		par2lpc [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of LPC		[25]		*
*	infile:								*
*		PARCOR							*
*		    , K, k(1), ..., k(m),				*
*	stdout:								*
*		LP Coefficeints						*
*		    , K, a(1), ..., a(m),				*
*	require:							*
*		par2lpc()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: par2lpc.c,v 1.1.1.1 2000/03/01 13:58:48 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void	par2lpc();


/*  Default Values  */
#define ORDER		25


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform PARCOR to LPC\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of LPC       [%d]\n", ORDER);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       PARCOR (float)             [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       LP coefficients (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER;
    FILE	*fp = stdin;
    double	*k, *a;
    
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
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    a = dgetmem(m+m+2);
    k = a + m + 1;

    while (freadf(k, sizeof(*k), m+1, fp) == m+1){
	par2lpc(k, a, m);
	fwritef(a, sizeof(*a), m+1, stdout);
    }
    exit(0);
}

