/************************************************************************
*									*
*    Generate Impulse Sequence						*
*									*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		impulse [options] > stdout				*
*	options:							*
*		-l l     :  length		 	[256]		*
*		-n n     :  order		 	[255]		*
*	stdout:								*
*		impulse sequence					*
*		    , 1, 0, 0, ...					*
*	notice:								*
*		if l < 0, generate infinite sequence			*
*									*
************************************************************************/

static char *rcs_id = "$Id: impulse.c,v 1.1 2000/03/01 13:58:37 yossie Exp $";


/*  Standard C Libralies  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>

/*  Default Values  */
#define LENG 		256


/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - generate impulse sequence\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : length             [%d]\n", LENG);
    fprintf(stderr, "       -n n  : order              [%d]\n", LENG-1);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       impulse sequence (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       if l < 0, generate infinite sequence\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		l = LENG, i;
    double	x = 1.0, y = 0.0;
    
    
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

    if (l==0) exit(0);
    else fwritef(&x, sizeof(x), 1, stdout);
    
    for(i=0;; i++){
	if(i == l-1) break;
	fwritef(&y, sizeof(y), 1, stdout);
    }
    exit(0);
}

