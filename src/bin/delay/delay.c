/************************************************************************
*									*
*    Delay Sequence							*
*									*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		delay [ options ] [ infile ] > stdout			*
*	options:							*
*		-s s     :  start sample		[0]		*
*		-f	 :  keep file size				*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ...					*
*	stdout:								*
*		delayed data						*
*		    , 0, 0, ..., 0, x(1), x(2), ..., 			*
*		      ------------					*
*			s sample					*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Default Values  */
#define START		0
#define KEEP		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - delay sequence\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -s s  : start sample      [%d]\n", START);
    fprintf(stderr, "       -f    : keep file length  [%s]\n", BOOL[KEEP]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)     [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       delayed data sequence (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		start = START, leng = 16384, i;
    FILE	*fp = stdin;
    double	*x;
    Boolean	keep = KEEP;
    
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 's':
		    start = atoi(*++argv);
		    --argc;
		    break;
		case 'f':
		    keep = 1 - keep;
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

    x = dgetmem(leng);
    while ((i = freadf(x+start, sizeof(*x), leng-start, fp)) == leng-start){
	fwritef(x, sizeof(*x), leng-start, stdout);
	movem(x+leng-start,x,sizeof(*x),start);
    }
    
    if (keep) fwritef(x,sizeof(*x),i,stdout);
    else fwritef(x, sizeof(*x), i+start, stdout);

    exit(0);
}

