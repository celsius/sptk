/************************************************************************
*									*
*    Histogram								*
*									*
*					1986.9	K.Tokuda		*
*					1995.7	K.Tokuda		*
*					1996.3  N.Miyazaki		*
*									*
*	usage:								*
*		histogram [ options ] > stdout				*
*	options:							*
*		-l l	 : sequence length		[256]		*
*		-i i	 : infimum			[0]		*
*		-j j	 : supremum                     [1]		*
*		-n n	 : step size			[0.1]		*
*	infile:								*
*		sample data (double)					*
*	stdout:								*
*		histogram (double)					*
*									*
************************************************************************/


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>


/*  Command Name  */
char	*cmnd;


int	usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - histogram\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : frame size         [0]\n");
	fprintf(stderr, "       -i i  : infimum            [0.0]\n");
	fprintf(stderr, "       -j j  : supremum           [1.0]\n");
	fprintf(stderr, "       -n n  : step size          [0.1]\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)      [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       histogram (int)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       if l > 0, calculate histogram frame by frame\n");
	fprintf(stderr, "\n");
	exit(1);
}

main(argc, argv)
	int argc;
	char *argv[];
{
	double	i=0, j=1, n=0.1;
	int	l=0, k, ii, *h;
	double	*x, xx;
	double atof();
	int atoi();
	FILE	*fp = stdin;
	
	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	        cmnd = argv[0];
	else
	        cmnd++;
	while (--argc)
		if (**++argv == '-'){
			argc--;
			switch (*(*argv+1)) {
			case 'l':
				l = atoi(*++argv);
				break;
			case 'i':
				i = atof(*++argv);
				break;
			case 'j':
				j = atof(*++argv);
				break;
			case 'n':
				n = atof(*++argv);
				break;
			case 'h':
			default:
				usage();
			}
		} else	fp = getfp(*argv, "r");

	k = (int)((j-i)/n + 1.0);
	h = (int *)dgetmem(k+2);


	if (l) {
		x = dgetmem(l);
		if ( freadf(x, sizeof(*x), l, fp) != l) {
			fprintf(stderr, "input too short!\n");
			exit(1);
		}
		histogram(x, l, i, j, n, h);
		fwrite(h, sizeof(*h), k, stdout);
        } else {
		fillz(h, sizeof(*h), k+2);
		while (freadf(&xx, sizeof(xx), 1, fp) == 1){
			for (ii = 0; ii <= k; ii++)
				if (xx < i + ii * n){
					h[ii]++;
					break;
				}
			if (xx >= i + ii * n)
				h[ii]++;
		}
		fwrite(&h[1], sizeof(*h), k, stdout);
		if (h[0] || h[ii])
			exit(1);
	}	
	exit(0);
}
