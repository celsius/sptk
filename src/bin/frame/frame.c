/************************************************************************
*									*
*    Extract Frame from Data Sequence					*
*									*
*					1985.11 K.Tokuda		*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		frame [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l     :  frame length		[256]		*
*		-p p     :  frame period		[100]		*
*		-n	 :  no center start point	[FALSE]		*
*		+type    :  data type 			[f]		*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*									*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., 					*
*	stdout:								*
*		frame sequence						*
*		    0, 0, ..., 0, x(0), x(1), ..., x(l/2),		*
*		    , x(t), x(t+1),       ...,       x(t+l-1),		*
*		    , x(2t), x(2t+1),     ....				*
*		if -n specified						*
*		    x(0), x(1),           ...,       x(l),		*
*		    , x(t), x(t+1),       ...,       x(t+l-1),		*
*		    , x(2t), x(2t+1),     ....				*
*									*
************************************************************************/

static char *rcs_id = "$Id: frame.c,v 1.1.1.1 2000/03/01 13:58:34 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Default Values  */
#define LENG 		256
#define	FPERIOD		100
#define NOCTR		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - extract frame from data sequence\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : frame length          [%d]\n", LENG);
    fprintf(stderr, "       -p p  : frame period          [%d]\n", FPERIOD);
    fprintf(stderr, "       -n    : no center start point [%s]\n", BOOL[NOCTR]);
    fprintf(stderr, "       +type : data type             [f]\n");
    fprintf(stderr, "                c (char)      s (short)\n");
    fprintf(stderr, "                i (int)       l (long)\n");
    fprintf(stderr, "                f (float)     d (double)\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence              [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       extracted data sequence\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		  l = LENG, fprd = FPERIOD, size = sizeof(float), ns, i;
    FILE	  *fp = stdin;
    Boolean	  noctr = NOCTR;
    char	  *x, *xx, *p1, *p2, *p;
    register char *s, c;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (*(s = *++argv) == '-') {
	    c = *++s;
	    switch (c) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'p':
		    fprd = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    noctr = 1 - noctr;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (*s == '+'){
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

    x = (char *)dgetmem(size*l);

    if(!noctr){
    i = (int)((l + 1) / 2);
    if(fread(&x[(int)(l/2)*size], size, i, fp) != i) 
	exit(0);
    } else {
	if(fread(x, size, l, fp) != l)
	    exit(0);
    }

    fwrite(x, size, l, stdout);

    if((ns = (l-fprd)) > 0){
	p = &x[fprd * size];
	for(;;){
	    p1 = x;  p2 = p;
	    i = ns * size;
	    while (i--) *p1++ = *p2++;
	    
	    if(fread(p1, size, fprd, fp) != fprd) break;
	    fwrite(x, size, l, stdout);
	}
    }
    else{
	i = -ns;
	xx = (char *)dgetmem(i*size);
	for(;;){
	    if(fread(xx, size, i, fp) != i) break;

	    if(fread(x, size, l, fp) != l) break;
	    fwrite(x, size, l, stdout);
	}
    }
    exit(0);
}

