/************************************************************************
*									*
*    Calculation of Cepstral Distance					*
*					1996.7  K.Koishida		*
*									*
*	usage:								*
*		cdist [ options ] file1 [ infile ] > stdout		*
*	options:							*
*		-m m     :  order of minimum-phase cepstrum	[25]	*
*		-o o     :  output format			[0]	*
*				0 ([dB])				*
*				1 (squared error)			*
*				2 (root squared error)			*
*		-f       :  frame length	  		[FALSE]	*
*	file1:								*
*	infile:								*
*		minimum-phase cepstrum					*
*		    , c(0), c(1), ..., c(m),				*
*	stdout:								*
*		cepstral distance					*
*									*
************************************************************************/

static char *rcs_id = "$Id: cdist.c,v 1.1.1.1 2000/03/01 13:58:30 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


#define	LN_TO_LOG	4.3429448190325182765


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
double	sqrt();


/*  Default Values  */
#define ORDER		25
#define FRAME		FA
#define OTYPE		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - calculation of cepstral distance\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "      -m m  : order of minimum-phase cepstrum [%d]\n", ORDER);
    fprintf(stderr, "      -o o  : output format                   [%d]\n", OTYPE);
    fprintf(stderr, "                0 ([dB])\n");
    fprintf(stderr, "                1 (squared error)\n");
    fprintf(stderr, "                2 (root squared error)\n");
    fprintf(stderr, "      -f    : output frame by frame           [%s]\n", BOOL[FRAME]);
    fprintf(stderr, "      -h    : print this message\n");
    fprintf(stderr, "  file1:\n");
    fprintf(stderr, "  infile:                                     [stdin]\n");
    fprintf(stderr, "      minimum-phase cepstrum (float)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "      cepstral distance (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, num = 0, otype = OTYPE, i;
    FILE	*fp = stdin, *fp1 = NULL;
    double	*x, *y, sub, sum, z = 0.0;
    Boolean	frame = FRAME;
    
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
		case 'o':
		    otype = atoi(*++argv);
		    --argc;
		    break;
		case 'f':
		    frame = 1 - frame;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
        else if (fp1 == NULL)
            fp1 = getfp(*argv, "r");
        else
            fp = getfp(*argv, "r");

    x = dgetmem(m+m+2);
    y = x + m + 1;

    while (freadf(x, sizeof(*x), m+1, fp) == m+1 &&
	   freadf(y, sizeof(*y), m+1, fp1)== m+1 ){

	sum = 0.0;
	for(i=1; i<=m; i++){
	    sub = x[i] - y[i];
	    sum += sub * sub;
	}

	if(otype == 0){
 	    sum = sqrt(2.0*sum);
	    sum *= LN_TO_LOG;
	}
	else if(otype == 2)
	    sum = sqrt(sum);

	if(! frame){
	    z += sum;
	    num++;
	}
	else
	    fwritef(&sum, sizeof(sum), 1, stdout);
    }

    if(! frame){
	z = z / (double)num;
	fwritef(&z, sizeof(z), 1, stdout);
    }
    
    exit(0);
}

