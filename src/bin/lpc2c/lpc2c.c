/************************************************************************
*									*
*    Transform LPC to Cepstrum						*
*									*
*					1988.7  T.Kobayashi		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		lpc2c [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of LPC		[25]		*
*		-M M     :  order of cepstrum		[25]		*
*	infile:								*
*		LP Coefficeints						*
*		    , K, a(1), ..., a(M),				*
*	stdout:								*
*		cepstral coefficients					*
*		    , c(0), c(1), ..., c(N),				*
*	require:							*
*		lpc2c()							*
*									*
************************************************************************/

static char *rcs_id = "$Id: lpc2c.c,v 1.1 2000/03/01 13:58:39 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void	lpc2c();


/*  Default Values  */
#define ORDERC		25
#define ORDERA		25


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform LPC to cepstrum\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of LPC       [%d]\n", ORDERA);
    fprintf(stderr, "       -M M  : order of cepstrum  [%d]\n", ORDERC);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       LP coefficiets (float)     [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       cepstrum (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDERA, n = ORDERC;
    FILE	*fp = stdin;
    double	*c, *a;
    
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
		case 'M':
		    n = atoi(*++argv);
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

    a = dgetmem(m+n+2);
    c = a + m + 1;

    while (freadf(a, sizeof(*a), m+1, fp) == m+1){
	lpc2c(a, m, c, n);
	fwritef(c, sizeof(*c), n+1, stdout);
    }
    exit(0);
}

