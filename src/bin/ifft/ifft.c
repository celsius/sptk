/************************************************************************
*									*
*    Inverse FFT for Complex Sequence					*
*									*
*	usage:								*
*		ifft [ infile ] [ options ] > stdout			*
*									*
*	options:							*
*		-l l	 :  FFT size power of 2		[256]		*
*		-R	 :  real part					*
*		-I	 :  imaginary part				*
*	infile:								*
*		stdin for default					*
*		input is assumed to be real				*
*									*
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <math.h>
#include <SPTK.h>
#include <string.h>

/*  Required Functions  */
int ifft(double*, double*, int);


static int	size = 256, out = ' ';


/*  Command Name  */
char	*cmnd;


int usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - inverse FFT for complex sequence\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : FFT size power of 2 [256]\n");
	fprintf(stderr, "       -R    : real part\n");
	fprintf(stderr, "       -I    : imaginary part\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)       [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       IFFT sequence (float)\n");
	fprintf(stderr, "\n");
	exit(1);
}

main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp, *fopen();
	char	*s, *infile = NULL, c;
	
	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	        cmnd = argv[0];
	else
	        cmnd++;
	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if( c == 'l' && *++s == '\0' ) {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				size = atoi(s);
				break;
			case 'i':
			case 'r':
				c -= ('a' - 'A');
			case 'I':
			case 'R':
				out = c;
				break;
			case 'h':
			default:
				usage();
			}
		}
		else
			infile = s;
	}

	if(infile) {
		fp = getfp(infile, "r");
		dft(fp);
		fclose(fp);
	}
	else
		dft(stdin);
	exit(0);
}

dft(fp)
FILE	*fp;
{
	double		*x, *y;
	register int	size2;
	static int	size_f;

	x = dgetmem(size2 = size + size);

	y = x + size;

	while(!feof(fp)) {
		if( freadf(x, sizeof(*x), size2, fp) != size2 )
				break;
			
		ifft(x, y, size);

		if(out != 'I')
			fwritef(x, sizeof(*x), size, stdout);
		if(out != 'R')
			fwritef(y, sizeof(*y), size, stdout);
	}
	return(0);
}
