/************************************************************************
*									*
*    Summation of Vectors						*
*									*
*					1988.7  T.Kobayashi		*
*					1996.5  K.Koishida		*
*	usage:								*
*		vsum [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l     :  order of vector			[1]	*
*		-n n     :  number of vector			[all]	*
*									*
************************************************************************/

static char *rcs_id = "$Id: vsum.c,v 1.1 2000/03/01 13:58:52 yossie Exp $";

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG		1


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - summation of vector\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l   : order of vector    [%d]\n",LENG);
    fprintf(stderr, "       -n n   : number of vector   [EOD]\n");
    fprintf(stderr, "       -h     : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       vectors (float)             [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       summation of vector (float)\n");
    fprintf(stderr, "\n");
    exit(status);
}


main(argc,argv)
int	argc;
char	*argv[];
{
    FILE	*fp = stdin;
    double      *x, *s;
    int	        leng = LENG, nv = -1, k, lp;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    leng = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    nv = atoi(*++argv);
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

    s = dgetmem(leng + leng);
    x = s + leng;
	
    while(! feof(fp)){
	for(k=0; k<leng; ++k)
	    s[k] = 0.0;
	for(lp=nv; lp; ) {
	    if(freadf(x, sizeof(*x), leng, fp) != leng)
		break;
	    for(k=0; k<leng; ++k)
		s[k] += x[k];
	    if(nv != -1)
		--lp;
	}
	if(lp == 0 || nv == -1)
	    fwritef(s, sizeof(*s), leng, stdout);
    }
    exit(0);
}
