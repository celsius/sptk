/************************************************************************
*									*
*    Calculation of Root Mean Squared Error				*
*									*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		rmse [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l     :  frame length	[entire data]		*
*			    if l>0, calculate rmse frame by frame	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		rmse							*
*		    , x(0)+x(1)...+x(l-1)/l				*
*	require:							*
*		rmse()							*
*									*
************************************************************************/

static char *rcs_id = "$Id: rmse.c,v 1.1.1.1 2000/03/01 13:58:50 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
double	rmse(), sqrt();


/*  Default Values  */
#define LENG		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - calculation of root mean squared error\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "      -l l  : frame length       [%d]\n", LENG);
    fprintf(stderr, "      -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "      data sequence (float)      [stdin]\n");
    fprintf(stderr, "  file1:\n");
    fprintf(stderr, "      data sequence (float)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "      root mean squared error (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "      if l > 0, calculate rmse frame by frame\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		l = LENG, num = 0;
    FILE	*fp = stdin, *fp1 = NULL;
    double	*x, *y, x1, y1, sub, z = 0.0;
    
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
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
        else if (fp1 == NULL)
            fp1 = getfp(*argv, "r");
        else
            fp = getfp(*argv, "r");

    if(l > 0) {
	x = dgetmem(l+l);
	y = x + l;
	while (freadf(x, sizeof(*x), l, fp) == l &&
	       freadf(y, sizeof(*y), l, fp1)== l ){
	    z = rmse(x, y, l);
	    fwritef(&z, sizeof(z), 1, stdout);
	}
    }
    else {
	while (freadf(&x1, sizeof(x1), 1, fp) == 1 &&
	       freadf(&y1, sizeof(y1), 1, fp1)== 1){
	    sub = x1 - y1;
	    z += sub * sub;

	    num++;
	}
	z = sqrt(z /= num);
	fwritef(&z, sizeof(z), 1, stdout);
    }
    exit(0);
}

