/************************************************************************
*									*
*    Improved Cepstral Analysis						*
*									*
*					1985.11 K.Tokuda		*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		icep [ options ] [ infile ] > stdout			*
*	options:							*
*		-m  m    :  order of cepstrum				*
*		-l  l    :  frame length				*
*		-j  j    :  number of iteration				*
*		-k  k    :  accelation factor				*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		cepstral coefficients					*
*		    , c(0), c(1), ..., c(m),				*
*	require:							*
*		icep()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
void	icep();


/*  Default Values  */
#define ORDER		25
#define FLNG		256
#define MAXITR		10
#define ACCELATION	1.0
#define EPS		0.0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - improved cepstral analysis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of cepstrum   [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : frame length        [%d]\n", FLNG);
    fprintf(stderr, "       -j j  : number of iteration [%d]\n", MAXITR);
    fprintf(stderr, "       -k k  : accelation factor   [%g]\n", ACCELATION);
    fprintf(stderr, "       -e e  : epsilon             [%g]\n", EPS);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       windowed sequence (float)   [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       cepstrum (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, l = FLNG, itr = MAXITR, i;
    double	ac = ACCELATION, eps = EPS, *x, *y, *c, log(), atof();
    FILE	*fp = stdin;
    
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
		case 'j':
		    itr = atoi(*++argv);
		    --argc;
		    break;
		case 'k':
		    ac = atof(*++argv);
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

    x = dgetmem(l+l+m+1);
    y = x + l; c = y + l;
    
    while (freadf(x, sizeof(*x), l, fp) == l){
	fftr(x, y, l);
	for(i=0; i<l; i++)
	    x[i] = log(x[i]*x[i] + y[i]*y[i] + eps);

	icep(x, l, c, m, itr, ac);
	
	fwritef(c, sizeof(*c), m+1, stdout);
    }
    exit(0);
}
