/************************************************************************
*									*
*    Transform  Cepstrum to Spectrum					*
*									*
*					1986. 9  K.Tokuda		*
*					1996. 4  K.Koishida		*
*									*
*	usage:								*
*		c2sp [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of cepstrum		[25]		*
*		-l l     :  frame length		[256]		*
*		-p       :  output phase  (see stdout)	[FALSE]		*
*		-o o     :  output format (see stdout)	[0]		*
*	infile:								*
*		cepstrum						*
*		    , c(0), c(1), ..., c(M),				*
*	stdout:								*
*	        output format       scale				*
*		       0 :	      20 * log|H(z)|			*
*		       1 :            ln|H(z)|				*
*		       2 :            |H(z)|				*
*	        (-p option is used)					*
*		       0 :	      arg|H(z)| / pi       [pi rad]	*
*		       1 :            arg|H(z)|            [rad]	*
*		       2 :            arg|H(z)| * 180 / pi [deg]	*
*		spectrum						*
*		    , s(0), s(1), ..., s(L/2),				*
*	require:							*
*		c2sp()							*
*									*
************************************************************************/

static char *rcs_id = "$Id: c2sp.c,v 1.1.1.1 2000/03/01 13:58:30 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Required Functions  */
void	c2sp();
double  exp(), log();


/*  Default Values  */
#define ORDER		25
#define LENG		256
#define PHASE		FA
#define OTYPE		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - transform cepstrum to spectrum\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of cepstrum    [%d]\n", ORDER);
    fprintf(stderr, "       -l l  : frame length         [%d]\n", LENG);
    fprintf(stderr, "       -p    : output phase         [%s]\n", BOOL[PHASE]);
    fprintf(stderr, "       -o o  : output format        [%d]\n", OTYPE);
    fprintf(stderr, "                0 (20*log|H(z)|)\n");
    fprintf(stderr, "                1 (ln|H(z)|)\n");
    fprintf(stderr, "                2 (|H(z)|)\n");
    fprintf(stderr, "               -p option is specified\n");
    fprintf(stderr, "                0 (arg|H(z)|/pi       [pi rad])\n");
    fprintf(stderr, "                1 (arg|H(z)|          [rad])\n");
    fprintf(stderr, "                2 (arg|H(z)|*180 / pi [deg])\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       cepstrum (float)             [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       spectrum (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int     leng = LENG, m = ORDER, start = 0, end = 0, otype = OTYPE,
    	    i, no;
    FILE    *fp = stdin;
    char    phase = PHASE;
    double  logk, *x, *y, *c;

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
		    leng = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    phase = 1 - phase;
		    break;
		case 'o':
		    otype = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    start = atoi(*++argv);
		    --argc;
		    break;
		case 'e':
		    end = atoi(*++argv);
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

    if(end == 0) end = m;
    
    x = dgetmem(leng*2);
    y = x + leng;
    c = dgetmem(m+1);

    no = leng / 2 + 1;
    logk = 20.0 / log(10.0);
    
    while (freadf(c, sizeof(*c), m+1, fp) == m+1){
	fillz(c, sizeof(*c), start);
	for (i=end+1; i<=m; i++)  c[i] = 0.0;
	
	c2sp(c, m, x, y, leng);

	if (phase)
	    switch (otype){
	        case 1 :
		    for (i = no; i--;) x[i] = y[i];
		    break;
		case 2 :
		    for (i = no; i--;)
			x[i] = y[i] * 180 / PI;
		    break;
		default :		
		    for (i = no; i--;)
			x[i] = y[i] / PI;
		    break;
		}
	else
	    switch (otype){
	        case 1 :
		    break;
                case 2 :
		    for (i = no; i--;)
			x[i] = exp(x[i]);
	            break;
		default :
		    for (i = no; i--;)
			x[i] *= logk;
	            break;
		}

	fwritef(x, sizeof(*x), no, stdout);
    }
    exit(0);
}
