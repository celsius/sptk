/************************************************************************
*									*
*    FFT for Real Sequence						*
*									*
*					1987	T.Kobayashi		*
*					1995.12 N.Isshiki   modified	*
*									*
*	usage:								*
*		fftr [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l	 :  FFT size power of 2		[256]		*
*		-m m	 :  order of sequence           [l-1]		*
*		-A	 :  amplitude					*
*		-R	 :  real part					*
*		-I	 :  imaginary part				*
*		-H	 :  output half size				*
*		-P	 :  power					*
*	infile:								*
*		stdin for default					*
*		input is assumed to be float				*
*									*
************************************************************************/

static char *rcs_id = "$Id:";

/* Standard C Libraries */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>


/* Required Function */
int 	fftr();


/* Default Values */
#define	SIZE	256


/* Command Name */
char 	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - FFT for real sequence\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : FFT size power of 2 [%d]\n", SIZE);
	fprintf(stderr, "       -m m  : order of sequence   [l-1]\n");
	fprintf(stderr, "       -A    : amplitude\n");
	fprintf(stderr, "       -R    : real part\n");
	fprintf(stderr, "       -I    : imaginary part\n");
	fprintf(stderr, "       -P    : power\n");
	fprintf(stderr, "       -H    : output half size\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)       [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       FFT sequence (float)\n");
	fprintf(stderr, "\n");
	exit(status);
}

main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	char	*s, *infile = NULL, c;
	int	size = SIZE, nout = 0, nd = -1, out = ' ';	

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	        cmnd = argv[0];
	else
	        cmnd++;
	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if((c == 'l' || c == 'm') && *++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				size = atoi(s);
				break;
			case 'm':
				nd = atoi(s) + 1;
				break;
			case 'H':
				nout = 1;
				break;
			case 'i':
			case 'p':
			case 'r':
				c -= ('a' - 'A');
#ifdef	AOP
			case 'A':
#endif
			case 'I':
			case 'P':
			case 'R':
				out = c;
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr,
					"%s: unknown option '%c'\n", cmnd, c);
				usage(1);
			}
		}
		else
			infile = s;
	}

	if(nd == -1) nd = size;
	if(nd > size) {
		fprintf(stderr, "%s: oder of sequence > FFT size\n", cmnd);
		exit(1);
	}
	nout = (nout) ? size / 2 + 1 : size;
	if(infile) {
		fp = getfp(infile, "r");
		dft(fp,size,nd,out,nout);
		fclose(fp);
	}
	else
		dft(stdin,size,nd,out,nout);
	exit(0);
}

dft(fp,size,nd,out,nout)
FILE	*fp;
int	size,nd,out,nout;
{
	double	*x, *y;
	register int	k;
#ifdef	AOP
	double	sqrt();
#endif

	x = dgetmem(size + size);
	y = x + size;

	while(!feof(fp)) {
		fillz(x, size, sizeof(double));
		if(freadf(x, sizeof(*x), nd, fp) == 0)
			break;
		fftr(x, y, size);
		if(out == 'P')
			for(k = 0; k < size; ++k)
				x[k] = x[k] * x[k] + y[k] * y[k];
#ifdef	AOP
		else if(out == 'A')
			for(k = 0; k < size; ++k)
				x[k] = sqrt(x[k] * x[k] + y[k] * y[k]);
#endif
		if(out != 'I')
			fwritef(x, sizeof(*x), nout, stdout);
		if(out == ' ' || out == 'I')
			fwritef(y, sizeof(*y), nout, stdout);
	}
	return(0);
}

