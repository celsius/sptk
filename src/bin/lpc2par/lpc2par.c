/************************************************************************
*									*
*    Transform LPC to PARCOR						*
*									*
*					1988    K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		lpc2par [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of LPC			[25]	*
*		-g g     :  gamma of generalized cepstum	[1.0]	*
*		-s       :  check stable or unstable		[FALSE]	*
*	infile:								*
*		LP Coefficeints						*
*		    , K, a(1), ..., a(m),				*
*	stdout:								*
*		PARCOR							*
*		    , K, k(1), ..., k(m),				*
*	require:							*
*		lpc2par()						*
*       notice:                                                         *
*               if g > 1.0, g = -1 / g .                                *
*									*
************************************************************************/

static char *rcs_id = "$Id: lpc2par.c,v 1.1.1.1 2000/03/01 13:58:40 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
int	lpc2par();


/*  Default Values  */
#define ORDER		25
#define GAMMA		1.0
#define STABLE		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform LPC to PARCOR\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of LPC                 [%d]\n", ORDER);
    fprintf(stderr, "       -g g  : gamma of generalized cepstum [%g]\n", GAMMA);
    fprintf(stderr, "       -s    : check stable or unstable     [%s]\n", BOOL[STABLE]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  stdin\n");
    fprintf(stderr, "       LP coefficients (float)              [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       PARCOR (float) or \n");
    fprintf(stderr, "       0 <stable>, -1 <unstable> (int) if -s option is specified\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if g > 1.0, g = -1 / g\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		m = ORDER, stable, i;
    FILE	*fp = stdin;
    double	*k, *a, g = GAMMA, atof();
    Boolean	flags = STABLE;
    
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
	        case 'g':
		    g = atof(*++argv);
		    --argc;
		    if (g > 1.0) g = -1.0 / g;
		    break;
		case 's':
		    flags = 1 - flags;
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

    while (freadf(a, sizeof(*a), m+1, fp) == m+1){
	if(g != 1.0)
	    for(i=1; i<=m; i++) a[i] *= g;

	stable = lpc2par(a, k, m);

	if(flags)
	    fwrite(&stable, sizeof(stable), 1, stdout);
	else
	    fwritef(k, sizeof(*k), m+1, stdout);
    }
    exit(0);
}

