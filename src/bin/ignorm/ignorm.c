/************************************************************************
*									*
*	Inverse Gain Normalization					*
*					1995.11  T.Masuko		*
*									*
*	usage:								*
*		ignorm [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of generalized cepstrum	[25]	*
*		-g g     :  gamma 				[0.0]	*
*	infile:								*
*		normalized generalized cepstral coefficients		*
*		    , K, c'(1), ..., c'(M),				*
*	stdout:								*
*		generalized cepstral coefficients			*
*		    , c~(0), c~(1), ..., c~(M),				*
*	notice:								*
*		if g > 1.0, g = -1 / g .				*
*	require:							*
*		ignorm(),						*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void ignorm();


/*  Default Values  */
#define	GAMMA		0.0
#define ORDER		25


/*  Command Name  */
char	*cmnd;


usage(status)
int	status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - inverse gain normalization\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -m m  : order of generalized cepstrum   [%d]\n", ORDER);
	fprintf(stderr, "       -g g  : gamma                           [%g]\n", GAMMA);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       normalized generalized cepstrum (float) [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       generalized cepstrum (float)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       if g > 1.0, g = -1 / g \n");
	fprintf(stderr, "\n");
	exit(status);
}

main(argc, argv)
int	argc;
char	**argv;
{
    double	g = GAMMA, *c, atof();
    int		m = ORDER;
    FILE	*fp = stdin;
    

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
	        case 'g':
		    g = atof(*++argv);
		    --argc;
		    if (g > 1.0) g = -1.0 / g;
		    break;
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
    
    c = dgetmem(m+1);
    
    while (freadf(c, sizeof(*c), m+1, fp) == m+1) {
	ignorm(c, c, m, g);
	fwritef(c, sizeof(*c), m+1, stdout);
    }
    exit(0);
}

