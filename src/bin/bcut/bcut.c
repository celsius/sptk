/************************************************************************
*									*
*    Binary File Cut							*
*									*
*					1990.3  K.Tokuda		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		bcut [options] [infile] > stdout			*
*	options:							*
*		-s s     :  start number                        [0]	*
*		-e e     :  end number                          [EOF]	*
*		-l l     :  block length                        [1]	*
*		-n n     :  block order                         [l-1]	*
*		+type    :  data type 				[c]	*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*	infile:								*
*		data sequence					[stdin] *
*	stdout:								*
*		cut data sequence					*
*	note:								*
*		When both -L and -n are specified,			*
*		latter argument is adopted.				*
*									*
************************************************************************/

static char *rcs_id = "$Id: bcut.c,v 1.1.1.1 2000/03/01 13:58:29 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define START		0
#define END		-1
#define LENG		1


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - binary file cut\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -s s  : start number [%d]\n",START);
    fprintf(stderr, "       -e e  : end number   [EOF]\n");
    fprintf(stderr, "       -l l  : block length [%d]\n",LENG);
    fprintf(stderr, "       -n n  : block order  [l-1]\n");
    fprintf(stderr, "       +type : data type    [f]\n");
    fprintf(stderr, "                c (char)      s (short)\n");
    fprintf(stderr, "                i (int)       l (long)\n");
    fprintf(stderr, "                f (float)     d (double)\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence        [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       cut data sequence\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       When both -l and -n are specified,\n");
    fprintf(stderr, "       latter argument is adopted.\n");
    fprintf(stderr, "\n");

    exit(status);
}


void main(int argc, char **argv)
{
    int		   size = sizeof(float), n = LENG;
    long 	   start = START, end = END, ptr, atol();
    FILE 	   *fp = stdin;
    register char  *s, c;
    double     	   x;
    
    
     if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    switch(c) {
		case 's':
		    start = atol(*++argv);
		    --argc;
		    break;
		case 'e':
		    end = atol(*++argv);
		    --argc;
		    break;
		case 'l':
		    n = atol(*++argv);
		    --argc;
		    break;
		case 'n':
		    n = atol(*++argv)+1;
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (*s == '+') {
	    c = *++s;
	    switch(c) {
		case 'c':
		    size = sizeof(char);
		    break;
		case 's':
		    size = sizeof(short);
		    break;
		case 'l':
		    size = sizeof(long);
		    break;
		case 'i':
		    size = sizeof(int);
		    break;
		case 'f':
		    size = sizeof(float);
		    break;
		case 'd':
		    size = sizeof(double);
		    break;
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
	    }
	}
	else
	    fp = getfp(*argv, "r");

    ptr = start * n;
    while(ptr--)
	if(fread(&x, size, 1, fp) != 1) exit(0);
    
    ptr = (end - start + 1) * n;
    while (end == -1 || ptr--){
	if (fread(&x, size, 1, fp) != 1)
	    break;
	fwrite(&x, size, 1, stdout);
    }

    exit(0);
}
