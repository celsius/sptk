/************************************************************************
*									*
*    	echo arguments to the standard error				*
*									*
*					2000.5  T.Yoshimura		*
*									*
*	usage:								*
*		echo2 [ options ] [ argument... ]			*
*	options:							*
*		-n  :  do not add the NEWLINE to the output. [FALSE]	*
*									*
************************************************************************/

static char *rcs_id = "$Id: echo2.c,v 1.2 2000/05/19 01:58:36 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Default Values  */
#define NEWLINE		TR


/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - echo arguments to the standard error\n",cmnd)
;
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ]\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -n    : no output newline   [%s]\n", BOOL[NEWLINE]);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(short argc, char **argv)
{
    int  newline = NEWLINE;

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc){
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'n':
		    newline = 1 - newline;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	} else {
	    fputs(*argv, stderr);
	    if (argc > 1) putc(' ', stderr);
	}
    }

    if (newline) putc('\n', stderr);
    
    exit(0);
}
