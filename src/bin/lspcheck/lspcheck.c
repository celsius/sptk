/************************************************************************
*									*
*    Check and Rearrange LSP					 	*
*									*
*					1996.6  K.Koishida		*
*									*
*	usage:								*
*		lspcheck [ options ] [ infile ] > stdout		*
*	options:							*
*		-m m     :  order of LSP		[25]		*
*		-s s     :  sampling frequency (kHz)	[10]		*
*		-k	 :  input & output gain		[FALSE]		*
*		-i i     :  input format (see infile)	[0]		*
*		-o o     :  output format (see infile)	[i]		*
*		-r	 :  rearrange LSP		[FALSE]		*
*	infile:								*
*	      input format	LSP					*
*		    0		normalized frequency (0 ~ pi)		*
*		    1		normalized frequency (0 ~ 0.5)		*
*		    2		frequency (kHz)				*
*		    3		frequency (Hz)				*
*		LSP							*
*		    , f(1), ..., f(m),					*
*	stdout:								*
*		frame number of irregular LSP of			*
*		rearranged LSP if -r option is specified		*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDER		25
#define ITYPE		0
#define OTYPE		-1
#define SAMPLING	10
#define ARRANGE		FA
#define GAIN		TR

/*  Command Name  */
char	*cmnd;

/*  Required Functions  */
int lspcheck();


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - check and rearrange LSP\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of LPC        [%d]\n", ORDER);
    fprintf(stderr, "       -s s  : sampling frequency  [%d]\n", SAMPLING);
    fprintf(stderr, "       -k    : input & output gain [%s]\n", BOOL[GAIN]);
    fprintf(stderr, "       -i i  : input format        [%d]\n", ITYPE);
    fprintf(stderr, "       -o o  : output format       [i]\n");
    fprintf(stderr, "                 0 (normalized frequency <0...pi>)\n");
    fprintf(stderr, "                 1 (normalized frequency <0...0.5>)\n");
    fprintf(stderr, "                 2 (frequency (kHz))\n");
    fprintf(stderr, "                 3 (frequency (Hz))\n");
    fprintf(stderr, "       -r    : rearrange LSP       [%s]\n", BOOL[ARRANGE]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       LSP (float)                 [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       frame number of irregular LSP or\n");
    fprintf(stderr, "       rearranged LSP (float) if -r option is specified\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		m = ORDER, sampling = SAMPLING, itype = ITYPE, 
		otype = OTYPE, i, num;
    Boolean     arrange = ARRANGE, gain = GAIN;
    FILE	*fp = stdin;
    double	*lsp, *lsp1;

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
		case 's':
		    sampling = atoi(*++argv);
		    --argc;
		    break;
		case 'i':
		    itype = atoi(*++argv);
		    --argc;
		    break;
		case 'o':
		    otype = atoi(*++argv);
		    --argc;
		    break;
		case 'r':
		    arrange = 1 - arrange;
		    break;
		case 'k':
		    gain = 1 - gain;
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

    if(otype < 0)
	otype = itype;
	
    lsp = dgetmem(m+m+gain);
    lsp1 = lsp + m + gain;
    
    num = 0;
    while (freadf(lsp, sizeof(*lsp), m+gain, fp) == m+gain){
	if(itype == 0)
	    for(i=gain; i<m+gain; i++)
		lsp1[i] = lsp[i] / PI2;
	else if (itype == 2 || itype ==3)
	    for(i=gain; i<m+gain; i++)
		lsp1[i] = lsp[i] / sampling;
	
	if(itype == 3)
	    for(i=gain; i<m+gain; i++)
		lsp1[i] = lsp[i] / 1000;
	    
	if(lspcheck(lsp1+gain, m) == -1){
	    if(! arrange){
		printf("frame number : %d\n",num);
		for(i=0; i<m+gain; i++)
		    printf("%f\n",lsp[i]);
		printf("\n");
	    }
	    else{
		lsparrange(lsp1+gain, m);

		if (otype == 0)
		    for(i=gain; i<m+gain; i++)
			lsp1[i] *= PI2;
		else if (otype == 2 || otype == 3)
		    for(i=gain; i<m+gain; i++)
			lsp1[i] *= sampling;

		if(otype == 3)
		    for(i=gain; i<m+gain; i++)
			lsp1[i] *= 1000;

		fwritef(lsp1, sizeof(*lsp1), m+gain, stdout);
	    }
	}
	else if(arrange){
	    if(itype == otype)
		fwritef(lsp, sizeof(*lsp), m+gain, stdout);
	    else{
		if (otype == 0)
		    for(i=gain; i<m+gain; i++)
			lsp1[i] *= PI2;
		else if (otype == 2 || otype == 3)
		    for(i=gain; i<m+gain; i++)
			lsp1[i] *= sampling;

		if(otype == 3)
		    for(i=gain; i<m+gain; i++)
			lsp1[i] *= 1000;
		fwritef(lsp, sizeof(*lsp), m+gain, stdout);
	    }
	}
	num++;
    }
    exit(0);
}

