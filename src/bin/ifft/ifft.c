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
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>
#include <string.h>

/*  Required Functions  */
int dft(FILE *fp);

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
#ifdef SPTK_VERSION
	fprintf(stderr, "\n");
	fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc,char *argv[])
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

int dft(FILE *fp)
{
	double		*x, *y;
	register int	size2;

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
