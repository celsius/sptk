/************************************************************************
*									*
*    Transform MLSA Digital Filter Coefficients to Mel Cepstrum 	*
*									*
*					1995.12  K.Koishida		*
*									*
*	usage:								*
*		b2mc [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m  :  order of mel cepstrum	[25]			*
*		-a a  :  all-pass constant	[0.35]			*
*	infile:								*
*		MLSA filter coefficients				*
*		    , b(0), b(1), ..., b(M),				*
*	stdout:								*
*		mel cepstral coefficients				*
*		    , c~(0), c~(1), ..., c~(M),				*
*	require:							*
*		b2mc()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void	b2mc();


/*  Default Values  */
#define	ALPHA		0.35
#define ORDER		25


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform MLSA digital filter coefficients\n",cmnd);
    fprintf(stderr, "                                    to mel-cepstrum \n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of mel cepstrum    [%d]\n", ORDER);
    fprintf(stderr, "       -a a  : all-pass constant        [%g]\n", ALPHA);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       MLSA filter coefficients (float) [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       mel-cepstrum (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		m = ORDER, m1;
    FILE	*fp = stdin;
    double	a = ALPHA, *x, atof();    

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
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else
	    fp = getfp(*argv, "r");

    m1 = m + 1;

    x = dgetmem(m1);

    while (freadf(x, sizeof(*x), m1, fp) == m1){
	b2mc(x, x, m, a);
	fwritef(x, sizeof(*x), m1, stdout);
    }
    exit(0);
}

