/************************************************************************
*									*
*    Linear Interpolation						*
*									*
*					1987.1  K.Tokuda		*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		linear-intpl [ options ] [ infile ] > stdout		*
*	options:							*
*		-l l         : output length               [256]	*
*		-m m         : number of interpolation     [255]	*
*		-x xmin xmax : minimum of x & maximum of x [0.0 0.5]	*
*	infile:								*
*		data sequence						*
*		    , x0, y0, x1, y1..., xK, yK				*
*			(x0 = xmin, xk = xmax)				*
*	stdout:								*
*		interpolated data					*
*		    , y0, ..., yn					*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG		255
#define MIN		0.0
#define MAX		0.5


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - linear interpolation\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l         : output length               [%d]\n", LENG+1);
    fprintf(stderr, "       -m m         : number of interpolation     [%d]\n", LENG);
    fprintf(stderr, "       -x xmin xmax : minimum of x & maximum of x [%d %d]\n",MIN,MAX);
    fprintf(stderr, "       -h           : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)                      [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       interpolated data sequence (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		l = LENG, ii, end;
    FILE	*fp = stdin;
    double	i = MIN, j = MAX, x1, y1, x2, y2, y, x, t, atof();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv)-1;
		    --argc;
		    break;
		case 'm':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'x':
		    i = atoi(*++argv);
		    --argc;
		    j = atoi(*++argv);
		    --argc;
		    break;
		case 'i':
		    i = atoi(*++argv);
		    --argc;
		    break;
		case 'j':
		    j = atoi(*++argv);
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

    t = (j - i) / (double) l;
    
    for(;;){
	if(freadf(&x2, sizeof(x2), 1, fp) != 1) break;
	if(freadf(&y2, sizeof(y2), 1, fp) != 1) break;
    
	fwritef(&y2, sizeof(y2), 1, stdout);
	
	for(end=0,ii=1; end==0; ){
	    x1 = x2;  y1 = y2;
	    if(freadf(&x2, sizeof(x2), 1, fp) != 1) break;
	    if(freadf(&y2, sizeof(y2), 1, fp) != 1) break;
	    
	    if(x2 == j) end = 1;
	    for(;; ii++){
		x = i + t * ii;
		if(x > x2) break;
		y = ((y1 - y2) * x + x1 * y2 - x2 * y1) / (x1 - x2);
		fwritef(&y, sizeof(y), 1, stdout);
	    }
	}
    }
    exit(0);
}

