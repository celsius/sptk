/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/************************************************************************
*									*
*    FFT for Complex Sequence						*
*									*
*					1995.12 N.Isshiki modified	*
*	usage:								*
*		fft [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l	 :  FFT size power of 2		[256]		*
*		-m m     :  order of sequence		[l-1]		*
*		-A	 :  amplitude			[FALSE]		*
*		-R	 :  real part			[FALSE]		*
*		-I	 :  imaginary part		[FALSE]		*
*		-P	 :  power			[FALSE]		*
*	infile:								*
*		stdin for default					*
*		input is assumed to be double				*
*									*
************************************************************************/

static char *rcs_id = "$Id: fft.c,v 1.3 2002/12/25 05:29:26 sako Exp $";

/* Standard C Libraries */
#include <stdio.h>
#include <SPTK.h>
#include <string.h>


/* Required Function */
int 	fft();


/* Default Values */
#define SIZE 	256


/* Command Name */
char 	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - FFT for complex sequence\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : FFT size  power of 2 [%d]\n", SIZE);
	fprintf(stderr, "       -m m  : order of sequence    [l-1]\n");
	fprintf(stderr, "       -A    : amplitude            [FALSE]\n");
	fprintf(stderr, "       -R    : real part            [FALSE]\n");
	fprintf(stderr, "       -I    : imaginary part       [FALSE]\n");
	fprintf(stderr, "       -P    : power                [FALSE]\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)        [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       FFT seqeunce (float)\n");
	fprintf(stderr, "\n");
	exit(status);
}


main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp;
	char	*s, *infile = NULL, c;
	int	size = SIZE, nd = -1, out = ' ';

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;

	while(--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			switch(c) {
			case 'l':
				size = atoi(*++argv);
				argc--;
				break;
			case 'm':
				nd = atoi(*++argv) + 1;
				argc--;
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
	if(infile) {
		fp = getfp(infile, "r");
		dft(fp,size,nd,out);
		fclose(fp);
	}
	else
		dft(stdin,size,nd,out);
	exit(0);
}

dft(fp,size,nd,out)
FILE	*fp;
int size,nd,out;
{
	double	*x, *y;
	register int	k ,size2;
#ifdef	AOP
	double	sqrt();
#endif

	x = dgetmem(size2 = size + size);
	y = x + size;

	while(!feof(fp)) {
		fillz(x, size2, sizeof(double));
		if(freadf(x, sizeof(*x), nd, fp) == 0)
			break;
		if(freadf(y, sizeof(*y), nd, fp) == 0)
			break;
		fft(x, y, size);
		if(out == 'P')
			for(k = 0; k < size; ++k)
				x[k] = x[k] * x[k] + y[k] * y[k];
#ifdef	AOP
		else if(out == 'A')
			for(k = 0; k < size; ++k)
				x[k] = sqrt(x[k] * x[k] + y[k] * y[k]);
#endif
		if(out != 'I')
			fwritef(x, sizeof(*x), size, stdout);
		if(out == ' ' || out == 'I')
			fwritef(y, sizeof(*y), size, stdout);
	}
	return(0);
}
