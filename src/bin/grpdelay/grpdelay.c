/************************************************************************
*									*
*    Group Delay of Digital Filter					*
*									*
*					1988.8	T.Kobayashi		*
*					1996.1  N.Isshiki modified	*
*									*
*	usage:								*
*		grpdelay [ options ] [ infile ] > stdout		*
*	options:							*
*		-l l	 :  FFT size power of 2		[256]		*
*		-m m	 :  order of filter		[l-1]		*
*		-a	 :  ARMA filter					*
*	infile:								*
*		stdin for default					*
*		input is asumed to be float				*
*	Note that double precision FFT is used.				*
************************************************************************/

static char *rcs_id = "$Id: grpdelay.c,v 1.1 2000/03/01 13:58:35 yossie Exp $";

/* Standard C Libraries */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>


/* Required Functions */
void	grpdelay();


/*Default Values */
#define	SIZE	256
#define	AMRA	0


/* Command Name  */
char	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - group delay of digital filter\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : FFT size power of 2 [%d]\n", SIZE);
	fprintf(stderr, "       -m m  : order of filter     [l-1]\n");
	fprintf(stderr, "       -a    : ARMA filter\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)       [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       group delay (float)\n");
	fprintf(stderr, "\n");
	exit(status);
}


main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	char	*s, *infile = NULL,c;
	int	size = SIZE, nd = -1, is_arma = AMRA;
	double	*x,*d;	

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	        cmnd = argv[0];
	else
	        cmnd++;
	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(c != 'a' && *++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'm':
				nd = atoi(s) + 1;
				break;
			case 'l':
				size = atoi(s);
				break;
			case 'a':
				is_arma = 1;
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr,
					"%s: unknown option '%c'\n", cmnd, c);
				break;
			}
		}
		else
			infile = s;
	}

	if(nd == -1) nd = size;
	if(nd > size) {
		fprintf(stderr, "%s: order of sequence > FFT size\n", cmnd);
		exit(1);
	}
	if(infile) {
		fp = getfp(infile, "r");
	}
	else
		fp = stdin;

	x =dgetmem(2 * size);
	d = x + size;
	while(!feof(fp)) {
		fillz(x, size, sizeof(*x));
		if(freadf(x, sizeof(*x), nd, fp) != nd)
			break;
		grpdelay(x,d,size,is_arma);
		fwritef(d, sizeof(*x), size/2 + 1, stdout);
	}
	exit(0);
}

