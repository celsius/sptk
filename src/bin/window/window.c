/************************************************************************
*									*
*    Data Windowing							*
*									*
*					1996.1	N.Miyazaki		*
*					1998.11	T.Masuko		*
*									*
*	usage:								*
*		window [ infile ] [ options ] > outfile			*
*	options:							*
*		-l l	 :  input frame length		[256]		*
*		-L L	 :  output frame length		[l]		*
*		-n n	 :  type of normalization	[1]		*
*			n=0: none					*
*			n=1: normalize by power				*
*			n=2: normalize by magnitude			*
*		-w w	 :  type of window				*
*			w=0: blackman    window				*
*			w=1: hamming     window				*
*			w=2: hanning     window				*
*			w=3: bartlett    window				*
*			w=4: trapezoid   window				*
*			w=5: rectangular window				*
*	infile:								*
*		stdin for default					*
*		input is assumed to be double				*
*	notice:								*
*		if L > l, (L-l)-zeros are padded			*
*									*
************************************************************************/
static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values */
#define	FLENG		256
#define	WINTYPE		BLACKMAN
#define	NORMFLG		1

/*  Required Function */
double	window();


/*  Command Name  */
char*	cmnd;

int usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - data windowing\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n"); 
	fprintf(stderr, "       %s [ options ] [ infile ] > outfile\n", cmnd); 
	fprintf(stderr, "  options:\n"); 
	fprintf(stderr, "       -l l  : frame length of input  [%d]\n", FLENG);
	fprintf(stderr, "       -L L  : frame length of output [l]\n");
	fprintf(stderr, "       -n n  : type of normalization  [%d]\n", NORMFLG);
	fprintf(stderr, "                 0 none\n");
	fprintf(stderr, "                 1 normalize by power\n");
	fprintf(stderr, "                 2 normalize by magnitude\n");
	fprintf(stderr, "       -w w  : type of window         [%d]\n", WINTYPE);
	fprintf(stderr, "                 0 (blackman)\n");
	fprintf(stderr, "                 1 (hamming)\n");
	fprintf(stderr, "                 2 (hanning)\n");
	fprintf(stderr, "                 3 (bartlett)\n");
	fprintf(stderr, "                 4 (trapezoid)\n");
	fprintf(stderr, "                 5 (rectangular)\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n"); 
	fprintf(stderr, "       data sequence (float)          [stdin]\n"); 
	fprintf(stderr, "  stdout:\n"); 
	fprintf(stderr, "       windowed data sequence (float)\n"); 
	fprintf(stderr, "\n");
	exit(1);
}


main(argc, argv)
int	argc;
char	*argv[];
{
	FILE	*fp = stdin;
	char	*s, c;
	int	fleng = FLENG, outl = -1, normflg = NORMFLG;
	Window	wintype = WINTYPE;
	double  *x, *zero;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;

	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0' && (c == 'n' || c == 'l' || c == 'w' || c == 'L')) {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'w':
				wintype = atoi(s);
				break;
			case 'l':
				fleng = atoi(s);
				break;
			case 'L':
				outl = atoi(s);
				break;
			case 'n':
				normflg = atoi(s);
				break;
			case 'h':
			default:
				usage();
			}
		}
		else
			 fp = getfp(*argv, "r");
	}

	if (outl < 0)
	    outl = fleng;

	x = dgetmem(fleng > outl ? fleng : outl);

	while(freadf(x, sizeof(*x), fleng, fp) == fleng) {
	    window(wintype, x, fleng, normflg);
	    fwritef(x, sizeof(*x), outl, stdout);
	}

	exit(0);
}

