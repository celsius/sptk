/************************************************************************
*									*
*    	concatenate and display files to the standard error 	        *
*									*
*					      2000.6  S.Sako	        *
*									*
*	usage:								*
*		cat2 [ options ] [ infile1 ] [ infile2 ] ...		*
*	options:							*
*		-n  :  output with line number                 [FALSE]	*
*									*
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>

/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - concatenate and display files to the standard error\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile1 ] [ infile2 ] ...\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -n    : output with line number    [FALSE]\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                      [stdin]\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "\n");
    exit(status);
}

int main(int argc, char **argv)
{
    int  linenum = 0;
    int  stdinmode = 0;
    char buf[512];
    FILE *fp;

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc){
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'n':
		    linenum = 1;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	} else {
		stdinmode = -1;
		if( (fp = fopen( *argv, "r")) == NULL){
			fprintf(stderr, "%s: cannot open %s\n", cmnd, *argv);
			exit(1);
		}
		while( fgets( buf, 512, fp) != 0){
			if( linenum > 0) fprintf( stderr, "%6d  ", linenum++);
			fputs( buf, stderr);
		}
	}
    }

    if( stdinmode == 0 ){
	    while( fgets( buf, 512, stdin) != 0){
		    if( linenum > 0) fprintf( stderr, "%6d  ", linenum++);
	    fputs( buf, stderr);
	    }
    }
    exit(0);
}
