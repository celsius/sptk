/************************************************************************
*									*
*    Generate excitation 						*
*									*
*					1986.6  K.Tokuda		*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		excite [ options ] [ infile ] > stdout			*
*	options:							*
*		-p p     :  frame period			[100]	*
*		-i i     :  interpolation period		[1]	*
*		-n 	 :  gauss/M-sequence flag for unoiced	[FALSE]	*
*				default is M-sequence			*	
*		-s s	 :  seed for nrand			[1]	* 
*	infile:								*
*		pitch data						*
*	stdout:								*
*		excitation						*
*	require:							*
*		mseq()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
double	sqrt();
int     mseq();

double	rnd();
unsigned long	srnd();
double	nrandom();
int	nrand();


/*  Default Values  */
#define	FPERIOD		100
#define	IPERIOD		1
#define SEED		1
#define	RAND_MAX	32767
#define GAUSS		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - generate excitation\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -p p  : frame period                  [%d]\n", FPERIOD);
    fprintf(stderr, "       -i i  : interpolation period          [%d]\n", IPERIOD);
    fprintf(stderr, "       -n    : gauss/M-sequence for unvoiced [%s]\n", BOOL[GAUSS]);
    fprintf(stderr, "                   default is M-sequence\n");
    fprintf(stderr, "       -s s  : seed for nrand                [%d]\n",SEED);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       pitch period (float)         [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       excitation (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		fprd = FPERIOD, iprd = IPERIOD, i, j,seed=SEED;
    long	next = SEED;
    FILE	*fp = stdin;
    double	x, p1, p2, inc, pc;
    Boolean	gauss = GAUSS;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'p':
		    fprd = atoi(*++argv);
		    --argc;
		    break;
		case 'i':
		    iprd = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    gauss = TR;
		    break;
		case 's':
		    seed = atoi(*++argv);
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

    if(gauss & (seed  != 1)) next = srnd((unsigned)seed);

    if(freadf(&p1, sizeof(p1), 1, fp) != 1) exit(1);

    pc = p1;
    
    for(;;){
	if(freadf(&p2, sizeof(p2), 1, fp) != 1) exit(0);
	
	if(p1 != 0.0 && p2 != 0.0)
	    inc = (p2 - p1) * (double) iprd / (double) fprd;
	else{
	    inc = 0.0;
	    pc = p2;
	    p1 = 0.0;
	}
	
	for(j=fprd, i=(iprd+1)/2; j--;){
	    if(p1 == 0.0)
		if(gauss) x = (double)nrandom(&next);
		else x = mseq();
	    else{
		if((pc += 1.0) >= p1){
		    x = sqrt(p1);
		    pc = pc - p1;
		}
		else
		    x = 0.0;
	    }

	    fwritef(&x, sizeof(x), 1, stdout);

	    if (!--i){
		p1 += inc;
		i = iprd;
	    }
	}
	p1 = p2;
    }
    exit(0);
}

