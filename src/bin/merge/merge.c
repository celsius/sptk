/************************************************************************
*									*
*    Data Merge								*
*									*
*					1986.6  K.Tokuda		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		merge [options] file1 [file2] > stdout			*
*	options:							*
*		-s s     :  insert point                        [0]	*
*		-l l     :  frame length of input data          [25]    *
*		-n n     :  frame length of inserted data       [10]    *
*		-o       :  over write mode			[FALSE] *
*		+type    :  data type 				[f]	*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*									*
*	file1:  inserted data	, x(0), x(1), ..., x(l-1)		* 
* 	file2:	input data      , y(0), y(1), ..., y(n-1)		* 
*	stdout:								*
*		x(0), ..., x(s), y(0), ...,y(n-1), x(s+1), ..., x(n-1)  *
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Default Values  */
#define START		0
#define LENG1		25
#define LENG2		10
#define WRITE		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - data merge\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -s s  : insert point                [%d]\n",START);
    fprintf(stderr, "       -l l  : frame length of input data  [%d]\n",LENG1);
    fprintf(stderr, "       -n n  : l - 1                       [%d]\n",LENG1-1);
    fprintf(stderr, "       -L L  : frame length of insert data [%d]\n",LENG2);
    fprintf(stderr, "       -N N  : L - 1                       [%d]\n",LENG2-1);
    fprintf(stderr, "       -o    : over write mode             [%s]\n",BOOL[WRITE]);
    fprintf(stderr, "       +type : data type                   [f]\n");
    fprintf(stderr, "                c (char)      s (short)\n");
    fprintf(stderr, "                i (int)       l (long)\n");
    fprintf(stderr, "                f (float)     d (double)\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                       [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       merged data sequence\n");
    fprintf(stderr, "\n");

    exit(status);
}

void main(int argc, char **argv)
{
    FILE 	*fp2 = NULL, *fp1 = stdin;
    int 	size = sizeof(float), start = START, 
		leng1 = LENG1, leng2 = LENG2, i, j, flag = 1;
    Boolean	write = WRITE;
    char   	*y, c, *s;
    double	x;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    switch(c) {
		case 's':
		    start = atoi(*++argv);
		    --argc;
		    break;
		case 'l':
		    leng1 = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    leng1 = atoi(*++argv)+1;
		    --argc;
		    break;
		case 'L':
		    leng2 = atoi(*++argv);
		    --argc;
		    break;
		case 'N':
		    leng2 = atoi(*++argv)+1;
		    --argc;
		    break;
		case 'o':
		    write = 1 - write;
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
	else if (fp2 == NULL)
	    fp2 = getfp(*argv, "r");
	else
	    fp1 = getfp(*argv, "r");
    
    y = (char *)dgetmem(leng2 * size);
    
    for (;;){
	for (j = start, i = leng1; j-- && i--;){
	    if (fread(&x, size, 1, fp1) != 1)
		exit(1);
	    fwrite(&x, size, 1, stdout);
	}
	for (j = leng2; j--;)
	    if (write){
		if (fread(&x, size, 1, fp1) != 1)
		    exit(1);
		i--;
	    }
	if (fread(y, size, leng2, fp2) != leng2)
	    if (! flag)
		exit(1);

	fwrite(y, size, leng2, stdout);
	flag = 0;
	for (; i-- > 0;){
	    if (fread(&x, size, 1, fp1) != 1)
		break;
	    fwrite(&x, size, 1, stdout);
	}
    }
    exit(0);
}
