/************************************************************************
*									*
*    Solve an Autocorrelation Normal Equation				*
*			Using Levinson-Durbin Method			*
*									*
*					1987.9  K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		levdur [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of correlation	[25]		*
*	infile:								*
*		autocorrelation						*
*		    , r(0), r(1), ..., r(m),				*
*	stdout:								*
*		LP Coefficeints						*
*		    , K, a(1), ..., a(m),				*
*	require:							*
*		levdur()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: levdur.c,v 1.1 2000/03/01 13:58:38 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
int	levdur();


/*  Default Values  */
#define ORDER		25


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - solve an autocorrelation normal equation\n",cmnd);
    fprintf(stderr, "                    using Levinson-Durbin method\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of correlation [%d]\n", ORDER);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       autocorrelation (float)      [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       LP coefficients (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		m = ORDER, flag;
    FILE	*fp = stdin;
    double	*r, *a;
    
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
    r = a + m + 1;

    while (freadf(r, sizeof(*r), m+1, fp) == m+1){
	flag = levdur(r, a, m, -1.0);
	fwritef(a, sizeof(*a), m+1, stdout);
    }
    exit(0);
}

