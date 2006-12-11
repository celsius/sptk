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
*    2D-FFT for Real Sequence						*
*									*
*					1989	T.Kobayashi		*
*					1996.2  N.Miyazake modified	*
*									*
*	usage:								*
*		fftr2 [ infile ] [ options ] > stdout			*
*	options:							*
*		-l l	 :  FFT size. power of 2	[64]		*
*		-m m1 m2 :  region of support		[m1=64,m2=m1]   *
*		-t	 :  transposed output				*
*		-c	 :  compensated boundary			*
*		-q	 :  first quadrant output			*
*		-A	 :  amplitude					*
*		-R	 :  real part					*
*		-I	 :  imaginary part				*
*		-P       :  power					*
*	infile:								*
*		stdin for default					*
*		input is assumed to be double				*
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <math.h>
#include <string.h>


/*  Required Functions  */
int trans(double *p);

/*  Command Name  */
static char	*cmnd;


static int	size = 64, outopt = 0, n1 = 0, n2 = 0, out = ' ';

int usage()
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - 2D-FFT for real sequence\n", cmnd); 
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l      : FFT size power of 2   [64]\n");
    fprintf(stderr, "       -m m1 m2  : region of support     [m1=64, m2=m1]\n");
    fprintf(stderr, "       -t        : transposed output\n");
    fprintf(stderr, "       -c        : compensated boundary\n");
    fprintf(stderr, "       -q        : first quadrant output\n");
    fprintf(stderr, "       -A        : amplitude\n");
    fprintf(stderr, "       -R        : real part\n");
    fprintf(stderr, "       -I        : imaginary part\n");
    fprintf(stderr, "       -P        : power\n");
    fprintf(stderr, "       -h        : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)             [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       FFT sequence (float)\n");
    fprintf(stderr, "\n");
    exit(1);
}

int main(int argc,char *argv[])
{
	FILE	*fp;
	char	*s, *infile = NULL, c;
	double	*x, *y;
	register double	*xp, *yp;

	int		size2;
	register int	i, k;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	        cmnd = argv[0];
	else
	        cmnd++;
	while (--argc){
		if(*(s = *++argv) == '-') {
			c = *++s;
			if((c == 'l' || c == 'm') && *++s == '\0'){
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				size = atoi(s);
				break;
			case 'm':
				n1 = atoi(s);
				if ( argc == 1 ) {
					n2 = n1;
				} else {
					s = *++argv;				
					argc--;
					if ( *s >= '0' && *s <= '9') {
						n2 = atoi(s);
					} else {
						n2 = n1;
						s = *--argv;
						argc++;
					}
				}
				break;
			case 't':
			case 'c':
			case 'q':
				if(c == 't' || *++s == 't')
					outopt = 1;
				if(c == 'c' || *s == 'c')
					outopt = 2;
				if(c == 'q')
					outopt = -1;
				break;
			case 'a':
			case 'p':
			case 'r':
			case 'i':
				c -= ('a' - 'A');
			case 'A':
			case 'P':
			case 'R':
			case 'I':
				out = c;
				break;
			case 'h':
                                usage();
			default:
				usage();
			}
		}
		else
			infile = s;
	}

	if(n1 > size || n2 > size) {
		fprintf(stderr, "%s: Region size > FFT size\n", cmnd);
		exit(1);
	}

	if(infile)
		fp = getfp(infile, "r");
	else
		fp = stdin;

	
	size2 = size * size;

	x = dgetmem(2*size2);
	y = x + size2;


	size2 = size * size;

	x = dgetmem(2*size2);
	y = x + size2;

	while(!feof(fp)) {
		if(n1) {
			for(xp = x, k = n2; --k >= 0; xp += size) {
				if(freadf(xp, sizeof(*x), n1, fp) != n1)
					return(-1);
				if(n1 < size)
					fillz(xp + n1, sizeof(*xp), size - n1);
			}
		} else {
			if((k = freadf(x, sizeof(*x), size2, fp)) == 0)
				break;
			n2 = n1 = sqrt((double)k);
			if(k != n1 * n1) {
				fprintf(stderr, "%s: region of support is not square!\n", cmnd);
				return(-1);
			}
			if(n1 < size) {
				fillz(yp = x + size * n1, size * (size - n1));
				yp -= (size - n1);
				xp = x + k;
				for(k = n1; --k >= 0; yp -= (size - n1)) {
					fillz(yp, size - n1);
					for(i = n1; --i >= 0; )
						*--yp = *--xp;
				}
			}
		}
		
		fftr2(x, y, size);

		if(out == 'P')
			for(k = 0; k < size2; ++k)
				x[k] = x[k] * x[k] + y[k] * y[k];
		else if(out == 'A')
			for(k = 0; k < size2; ++k)
				x[k] = sqrt(x[k] * x[k] + y[k] * y[k]);
		if(out != 'I') {
			if(outopt)
				trans(x);
			else
				fwritef(x, sizeof(*x), size2, stdout);
		}
		if(out == ' ' || out == 'I') {
			if(outopt)
				trans(y);
			else
				fwritef(y, sizeof(*y), size2, stdout);
		}
	}

	free(x);
	exit(0);
}

int trans(double *p)
{
	int	k, sizeh, nout;
	register double	*q;

	sizeh = size / 2;
	nout = (outopt == 1) ? sizeh : sizeh + 1;

	if(outopt > 0)
		for(q = p + sizeh * size, k = sizeh; --k >= 0; q += size) {
			fwrite(q + sizeh, sizeof(*p), sizeh, stdout);
			fwrite(q, sizeof(*p), nout, stdout);
		}
	for(q = p, k = nout; --k >= 0; q += size) {
		if(outopt > 0)
			fwrite(q + sizeh, sizeof(*p), sizeh, stdout);
		fwrite(q, sizeof(*p), nout, stdout);
	}
}
