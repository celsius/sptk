/************************************************************************
*									*
*    Calculation of Average						*
*									*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		average [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l     :  frame length	[entire data]		*
*			    if l>0, calculate average frame by frame	*
*		-n n     :  order of sequence	[N/A]			*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		average							*
*		    , (x(0)+x(1)...+x(l-1))/l,				*
*	require:							*
*		average()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: average.c,v 1.1.1.1 2000/03/01 13:58:29 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>

/*  Required Functions  */
double	average();


/*  Default Values  */
#define LENG		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - calculation of average \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : frame length       [%d]\n", LENG);
    fprintf(stderr, "       -n n  : order of sequence  [l-1]\n", LENG);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)      [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       average (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if l > 0, calculate average frame by frame\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		l = LENG, num = 0;
    FILE	*fp = stdin;
    double	*x, xx, ave = 0.0;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    l = atoi(*++argv)+1;
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

    if(l > 0) {
	x = dgetmem(l);
	while (freadf(x, sizeof(*x), l, fp) == l){
	    ave = average(x, l);
	    fwritef(&ave, sizeof(ave), 1, stdout);
	}
    }
    else {
	while (freadf(&xx, sizeof(xx), 1, fp) == 1){
	    ave += xx;
	    num++;
	}
	ave /= num;
	fwritef(&ave, sizeof(ave), 1, stdout);
    }
    exit(0);
}

