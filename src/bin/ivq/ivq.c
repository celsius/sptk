/************************************************************************
*									*
*    	Decoder of Vector Quantization					*
*									*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		ivq [ options ] [ cbfile ] [ infile ] > stdout		*
*	options:							*
*		-l l      :  length of vector	     [26]		*
*		-n n      :  order of vector	     [25]		*
*	infile:								*
*		codebook index (int)					*
*	stdout:								*
*		quantized vector					*
*		    x'(0), x'(1), ..., x'(l-1),				*
*	require:							*
*		ivq()							*
*									*
************************************************************************/

static char *rcs_id = "$Id: ivq.c,v 1.1.1.1 2000/03/01 13:58:37 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <stdlib.h>
#include <string.h>


/*  Required Functions  */
void	ivq();


/*  Default Values  */
#define LENG		26
#define	CBSIZE		256


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - decoder of vector quantization \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] cbfile [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : length of vector   [%d]\n", LENG);
    fprintf(stderr, "       -n n  : order of vector    [%d]\n", LENG-1);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       index (int)                [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       quantized vector (float)\n");
    fprintf(stderr, "  cbfile:\n");
    fprintf(stderr, "       codebook (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


void main(int argc, char **argv)
{
    int		l = LENG, cbsize = CBSIZE, index;
    FILE	*fp = stdin, *fpcb = NULL;
    double	*x, *cb;

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-'){
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    l = atoi(*++argv) + 1;
		    --argc;
		    break;
		case 's':
		    cbsize = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (fpcb == NULL)
	    fpcb = getfp(*argv, "r");
	else
	    fp = getfp(*argv, "r");

    fseek(fpcb,0,2);
#ifdef DOUBLE
    cbsize = ftell(fpcb)/sizeof(double)/l;
#else
    cbsize = ftell(fpcb)/sizeof(float)/l;
#endif
    rewind(fpcb);

    x = dgetmem(l+cbsize*l);
    cb = x + l;

    if(freadf(cb, sizeof(*cb), cbsize*l, fpcb) != cbsize*l){
	fprintf(stderr,"%s : Codebook size error !\n",cmnd);
	exit(1);
    }
    
    while (fread(&index, sizeof(index), 1, fp) == 1){
	ivq(index, cb, l, x);
	fwritef(x, sizeof(*x), l, stdout);
    }
    exit(0);
}

