/************************************************************************
*									*
*    MLSA Digital Filter for Speech Synthesis				*
*									*
*					1994.6  T.Kobayashi		*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		mlsadf [ options ] mcfile [ infile ] > stdout		*
*	options:							*
*		-m m     :  order of cepstrum		 [25]		*
*		-a a     :  all-pass constant		 [0.35]		*
*		-p p     :  frame period		 [100]		*
*		-i i     :  interpolation period	 [1]		*
*		-b       :  mcep is filter coefficient b [FALSE]	*
*		-P P     :  order of pade approximation	 [4]		*
*		-k	 :  filtering without gain	 [FALSE]	*
*	infile:								*
*		mel cepstral coefficients				*
*		    , c~(0), c~(1), ..., c~(M),				*
*		excitation sequence					*
*		    , x(0), x(1), ..., 					*
*	stdout:								*
*		filtered sequence					*
*		    , y(0), y(1), ...,					*
*	notice:								*
*		P = 4 or 5						*
*	require:							*
*		mlsadf()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: mlsadf.c,v 1.1 2000/03/01 13:58:28 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
double	mlsadf(), exp();


/*  Default Values  */
#define ORDER		25
#define ALPHA		0.35
#define	FPERIOD		100
#define	IPERIOD		1
#define	BFLAG		FA
#define	PADEORDER	4
#define NGAIN		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - MLSA digital filter for speech synthesis\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] mcfile [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of mel-cepstrum        [%d]\n", ORDER);
    fprintf(stderr, "       -a a  : all-pass constant            [%g]\n", ALPHA);
    fprintf(stderr, "       -p p  : frame period                 [%d]\n", FPERIOD);
    fprintf(stderr, "       -i i  : interpolation period         [%d]\n", IPERIOD);
    fprintf(stderr, "       -b    : output filter coefficient b  [%s]\n", BOOL[BFLAG]);
    fprintf(stderr, "       -P P  : order of pade approximation  [%d]\n", PADEORDER);
    fprintf(stderr, "       -k    : filtering without gain       [%s]\n", BOOL[NGAIN]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       filter input (float)                 [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       filter output (float)\n");
    fprintf(stderr, "  mcfile:\n");
    fprintf(stderr, "       mel-cepstrum (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       P = 4 or 5 \n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, pd = PADEORDER, 
		fprd = FPERIOD, iprd = IPERIOD, i, j;
    FILE	*fp = stdin, *fpc = NULL;
    double	*c, *inc, *cc, *d, x, a = ALPHA, atof();
    Boolean	bflag = BFLAG, ngain = NGAIN;
    
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
		case 'a':
		    a = atof(*++argv);
		    --argc;
		    break;
		case 'p':
		    fprd = atoi(*++argv);
		    --argc;
		    break;
		case 'i':
		    iprd = atoi(*++argv);
		    --argc;
		    break;
		case 'P':
		    pd = atoi(*++argv);
		    --argc;
		    break;
		case 'b':
		    bflag = 1 - bflag;
		    break;
		case 'k':
		    ngain = 1 - ngain;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (fpc == NULL)
	    fpc = getfp(*argv, "r");
	else
	    fp = getfp(*argv, "r");

    if((pd < 4) || (pd > 5)){
	fprintf(stderr,"%s : Order of pade approximation is 4 or 5!\n",cmnd);
	exit(1);
    }

    if(fpc == NULL){
	fprintf(stderr,"%s : Cannot open mel cepstrum file!\n",cmnd);
	exit(1);
    }
	
    c = dgetmem(3*(m+1)+3*(pd+1)+pd*(m+2));
    cc  = c  + m + 1;
    inc = cc + m + 1;
    d   = inc+ m + 1;
    
    if(freadf(c, sizeof(*c), m+1, fpc) != m+1) exit(1);
    if(! bflag)
	mc2b(c, c, m, a);
	
    for(;;){
	if(freadf(cc, sizeof(*cc), m+1, fpc) != m+1) exit(0);
	if(! bflag)
	    mc2b(cc, cc, m, a);
	    
	for(i=0; i<=m; i++)
	    inc[i] = (cc[i] - c[i])* (double) iprd / (double) fprd;

	for(j=fprd, i=(iprd+1)/2; j--;){
	    if (freadf(&x, sizeof(x), 1, fp) != 1) exit(0);

	    if (!ngain) x *= exp(c[0]);
	    x = mlsadf(x, c, m, a, pd, d);
	    
	    fwritef(&x, sizeof(x), 1, stdout);
			
	    if (!--i){
		for (i=0; i<=m; i++) c[i] += inc[i];
		i = iprd;
	    }
	}

	movem(cc, c, sizeof(*cc), m+1);
    }
    exit(0);
}

