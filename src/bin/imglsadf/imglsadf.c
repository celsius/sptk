/************************************************************************
*									*
*    Inverse MGLSA Digital Filter					*
*									*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		imglsadf [ options ] [ infile ] > stdout		*
*	options:							*
*		-m m     :  order of mel-generalized cepstrum    [25]	*
*		-a a     :  alpha			       [0.35]	*
*		-g g     :  -1/gamma 			          [1]	*
*		-p p     :  frame period		        [100]	*
*		-i i     :  interpolation period	          [1]	*
*		-t       :  transpose filter		      [FALSE]	*
*		-k       :  filtering without gain	      [FALSE}	*
*	infile:								*
*		mel-generalized cepstral coefficients			*
*		    , c~(0), c~(1), ..., c~(M),				*
*		excitation sequence					*
*		    , x(0), x(1), ..., 					*
*	stdout:								*
*		filtered sequence					*
*		    , y(0), y(1), ...,					*
*	require:							*
*		imglsadf()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: imglsadf.c,v 1.1 2000/03/01 13:58:37 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
double	imglsadf(), imglsadft();


/*  Default Values  */
#define ORDER		25
#define ALPHA		0.35
#define STAGE		1
#define	FPERIOD		100
#define	IPERIOD		1
#define	TRANSPOSE	FA
#define NGAIN		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - inverse MGLSA digital filter\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] mgcfile [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of generalized cepstrum [%d]\n", ORDER);
    fprintf(stderr, "       -a a  : alpha                         [%g]\n", ALPHA);
    fprintf(stderr, "       -g g  : -1/gamma                      [%d]\n", STAGE);
    fprintf(stderr, "       -p p  : frame period                  [%d]\n", FPERIOD);
    fprintf(stderr, "       -i i  : interpolation period          [%d]\n", IPERIOD);
    fprintf(stderr, "       -t    : transepose filter             [%s]\n", BOOL[TRANSPOSE]);
    fprintf(stderr, "       -k    : filtering without gain        [%s]\n", BOOL[NGAIN]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       filter input (float)                  [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       filter output (float)\n");
    fprintf(stderr, "  mgcfile:\n");
    fprintf(stderr, "       mel-generalized cepstrum (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		m = ORDER, fprd = FPERIOD, iprd = IPERIOD, stage = STAGE, 
		i, j;
    Boolean	transpose = TRANSPOSE, ngain = NGAIN;
    FILE	*fp = stdin, *fpc = NULL;
    double	alpha = ALPHA, gamma, x, *c, *inc, *cc, *d, atof();
    
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
		    alpha = atof(*++argv);
		    --argc;
		    break;
 		case 'g':
		    stage = atoi(*++argv);
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
		case 't':
		    transpose = 1 - transpose;
		    break;
		case 'k':
		    ngain = 1 - ngain;
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

    if(fpc == NULL){
	fprintf(stderr,"%s : Cannot open cepstrum file!\n",cmnd);
	exit(1);
    }

    gamma = -1 / (double)stage;
    
    c = dgetmem(m+m+m+3+(m+1)*stage);
    cc  = c  + m + 1;
    inc = cc + m + 1;
    d   = inc+ m + 1;
    
    if(freadf(c, sizeof(*c), m+1, fpc) != m+1) exit(1);
    mc2b(c, c, m, alpha);
    gnorm(c, c, m, gamma);
    for(i=1; i<=m; i++)	
	c[i] *= gamma;

    for(;;){
	if(freadf(cc, sizeof(*cc), m+1, fpc) != m+1) exit(0);
	mc2b(cc, cc, m, alpha);
	gnorm(cc, cc, m, gamma);
	for(i=1; i<=m; i++)
	    cc[i] *= gamma;

	for(i=0; i<=m; i++)
	    inc[i] = (cc[i] - c[i])*iprd / fprd;

	for(j=fprd, i=(iprd+1)/2; j--;){
	    if (freadf(&x, sizeof(x), 1, fp) != 1) exit(0);

	    if (!ngain) x *= c[0];
	    if(transpose)
		x = imglsadft(x, c, m, alpha, stage, d);
	    else
		x = imglsadf(x, c, m, alpha, stage, d);
	    
	    
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

