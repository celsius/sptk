/************************************************************************
*									*
*    Extract Vector							*
*									*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		extract [ options ] [ indexfile ] [ infile ] > stdout	*
*	options:							*
*		-l l     :  order of vector	[10]			*
*		-i i     :  codebook index	[0]			*
*	infile:								*
*		index file (int)					*
*		    , index(0), index(1), ..., 				*
*		data sequence						*
*		    , x(0), x(1), ...,					*
*	stdout:								*
*		vectors which belong to index c				*
*		    , x'(0), x'(1), ...,				*
*									*
************************************************************************/

static char *rcs_id = "$Id: extract.c,v 1.1.1.1 2000/03/01 13:58:33 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG		10
#define INDEX		0
#define SIZE		256


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - extract vector \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] ifile [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : order of vector    [%d]\n", LENG);
    fprintf(stderr, "       -i i  : codebook index     [%d]\n", INDEX);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)      [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       extracted vector (float)\n");
    fprintf(stderr, "  ifile:\n");
    fprintf(stderr, "       indexfile (int)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		l = LENG, index = INDEX, size = SIZE, i;
    FILE	*fp = stdin, *fpi = NULL;
    double	*x;
    
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
		case 'i':
		    index = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    size = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
        else if (fpi == NULL)
            fpi = getfp(*argv, "r");
        else
            fp = getfp(*argv, "r");

    x = dgetmem(l);

    while (freadf(x, sizeof(*x), l, fp) == l &&
	   fread(&i, sizeof(i), 1, fpi)== 1 )
	if (i == index)
	    fwritef(x, sizeof(*x), l, stdout);

    exit(0);
}

