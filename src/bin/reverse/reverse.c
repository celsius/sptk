/************************************************************************
*									*
*    Reverse the order of Data in Each Block				*
*									*
*					1993.7	K.Tokuda		*
*					1996.1	N.miyazaki modified	*
*									*
*	usage:								*
*		reverse [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l	 : block length		[whole file]		*
*		-n n	 : block length - 1	[whole file]		*
*	infile:								*
*		x0, x1, ..., xN, (real)					*
*	stdout:								*
*		xN, ..., x1, x0, (real)					*
*									*
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define MAX 32767	/* 0x7fff */
int l_max = MAX;


/*  Required Functions  */
void	reverse();
void	usage();


/*   Command Name  */
char*	cmnd;


void usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - reverse the order of data in each block\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : block length       [whole file]\n");
	fprintf(stderr, "       -n n  : block length-1     [whole file]\n");
	fprintf(stderr, "       -h    : print this message\n");
 	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)      [stdin]\n");
 	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       reversed data sequence (float)\n");
	fprintf(stderr, "\n");
	exit(1);
}

main(argc, argv)
	int	argc;
	char	*argv[];
{
	int	l = -1;
	char	*s, c;
	double	*x, *p;
	int	max;
	FILE 	*fp = stdin;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;

	while(--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				l = atoi(s);
				break;
			case 'n':
				l = atoi(s) + 1;
				break;
			case 'h':
				usage();
			default :
				usage();
			}
		}
		else
		    fp = getfp(*argv, "r");
	}

	if (l > MAX){
		fprintf(stderr,
		"%s : -l L should be L <= %d !\n", cmnd, l_max);
		exit(1);
	}

	max = (l < 0) ? MAX : l;
	
	x = dgetmem(max);

	if (l < 0) {
		max = freadf(x, sizeof(*x), max, fp);
		reverse(x, max);
		fwritef(x, sizeof(*x), max, stdout);
	} else {
		while (freadf(x, sizeof(*x), l, fp) == l) {
			reverse(x, l);
			fwritef(x, sizeof(*x), l, stdout);
		}
	}
	exit(0);
}

