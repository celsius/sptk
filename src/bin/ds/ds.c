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
*                                                                       *
*       Sampling Rate Conversion (Down Sampling)                        *
*                                                                       *
*                                       1994. 5  T.Kobayashi            *
*                                       1997. 2  G.Hirabayashi          *
*					1998. 6  M.Tamura		*
*                                                                       *
*       usage:                                                          *
*               ds [ options ] [ infile ] > stdout                	*
*       options:                                                        *
*               -s s    :  conversion type              [21]            *
*                           21   down sampling by 2:1                   *
*                           43   down sampling by 4:3                   *
*                           52   down sampling by 5:2                   *
*                           54   down sampling by 5:4			*
*       infile:                                                         *
*               data sequence (float)                   [stdin]         *
*       stdout:                                                         *
*               converted data sequence (float)                         *
*       notice:                                                         *
*               Default LPF coefficients File                           *
*                2:1 -> /usr/local/cmnd/lib/lpfcoef.2to1                *
*                4:3 -> /usr/local/cmnd/lib/lpfcoef.4to3                *
*                5:2 -> /usr/local/cmnd/lib/lpfcoef.5to2up              *
*                5:2 -> /usr/local/cmnd/lib/lpfcoef.5to2dn              *
*                5:4 -> /usr/local/cmnd/lib/lpfcoef.5to2up              *
*                5:4 -> /usr/local/cmnd/lib/lpfcoef.5to2dn              *
*                                                                       *
************************************************************************/

/* Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SPTK.h>


/* Default Values */
#define DECRATE5	5
#define INTRATE5	2
#define	DECRATE4_3	4
#define	INTRATE4_3	3
#define DECRATE2_1	2
#define INTRATE2_1	1

#define	RBSIZE	512
#define	SIZE	256
#define STYPE 	21

#define BUFLENGTH 256

#ifndef LIB
	#define LIB "/usr/local/SPTK/lib"
#endif

#define COEF2_1 LIB "/lpfcoef.2to1"
#define	COEF4_3	LIB "/lpfcoef.4to3"
#define COEF5UP LIB "/lpfcoef.5to2up"
#define COEF5DN LIB "/lpfcoef.5to2dn"

#define	mod(x)	((x) & (RBSIZE -1))

static char	*cmnd, *coef, *coef1;
static int	start = -1, intrate, decrate, type = STYPE;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - sampling rate conversion (down sampling)\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -s s  : conversion type              [%d]\n",STYPE);
    fprintf(stderr, "                21  down sampling by 2:1\n");
    fprintf(stderr, "                43  down sampling by 4:3\n");
    fprintf(stderr, "                52  down sampling by 5:2\n");
    fprintf(stderr, "                54  down sampling by 5:4\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)                [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       conversioned data sequence (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "       Default LPF coefficients File\n");
    fprintf(stderr, "        2:1 -> %s\n", COEF2_1);
    fprintf(stderr, "        4:3 -> %s\n", COEF4_3);
    fprintf(stderr, "        5:2 -> %s\n", COEF5UP);
    fprintf(stderr, "        5:2 -> %s\n", COEF5DN);
    fprintf(stderr, "        5:4 -> %s\n", COEF5UP);
    fprintf(stderr, "        5:4 -> %s\n", COEF5DN);
    fprintf(stderr, "\n");
    exit(status);
}
    

main(argc,argv)
int	argc;
char	*argv[];
{
	FILE	*fp = stdin;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
	else
		cmnd++;

	while(--argc)
		if(*(*++argv) == '-') {
			switch(*(*argv+1)) {
			case 's':
				type = atoi(*++argv);
				--argc;
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr, "%s: unknown option '%c'\n", cmnd, *(*argv+1));
				usage(1);
			}
		}
		else
			fp  = getfp(*argv,"r");

	switch(type){
		case 21:
			coef = COEF2_1;
			decrate = DECRATE2_1;
			intrate = INTRATE2_1;
			break;
		case 43:
			coef = COEF4_3;
			decrate = DECRATE4_3;
			intrate = INTRATE4_3;
			break;
		case 52:
		case 54:
			coef = COEF5DN;
			coef1 = COEF5UP;
			decrate = DECRATE5;
			intrate = INTRATE5;
			break;
		default:
			fprintf(stderr, "%s: dec/int rate not specifiec.\n",cmnd);
			exit(1);
	}
	decimate(fp);
	fclose(fp);

	exit(0);
}

decimate(fp)
FILE	*fp;
{
	int	is_cont, i, k, nread, count, nwr, delay;
	double	x[SIZE], y[SIZE], firout();
	void	firin(), firin0(), firinit();

	firinit();
	delay = start;
	for(count = is_cont = 1; is_cont; ) {
		if((nread = freadf(x, sizeof(*x), SIZE, fp)) == 0) {
			is_cont = 0;
			nread = (decrate * start) / intrate;
			if(type == 54) nread /= 2;
			for(k = 0; k < nread; ++k)
				x[k] = 0;
		}
		for(nwr = k = 0; k < nread; ++k) {
			firin(x[k]);
			for(i = 0; i < intrate; ++i) {
				if(--count == 0) {
					y[nwr++] = firout(i);
					count = decrate;
				}
			}
			if(type == 54){
				firin0();
				for(i = 0; i < intrate; ++i) {
					if(--count == 0) {
						y[nwr++] = firout(i);
						count = decrate;
					}
				}
			}
		}
		if(delay) {
			if(nwr > delay) {
				nwr -= delay;
				if(fwritef(y + delay, sizeof(*y), nwr, stdout) != nwr){
					fprintf(stderr, "%s: write error\n", cmnd);
					exit(1);
				}
				delay = 0;
			} else {
				delay -= nwr;
			}
		} else {
			if(fwritef(y, sizeof(*y), nwr, stdout) != nwr){
				fprintf(stderr, "%s: write error\n", cmnd);
				exit(1);
			}
		}
	}
}

static double	rb[RBSIZE], rb2[RBSIZE], hdn[RBSIZE + 1], hup[RBSIZE + 1];
static int	flengdn = -1, flengup = -1, indx = 0, indx2 = 0;

void firin(in)
double	in;
{
	double	out;
	int	k,l;

	indx = mod(indx - 1);

	switch (type){
		case 52:
			rb2[indx] = in;
			out = 0;
			for(k = 0, l = indx ; k <= flengup; ++k, l = mod(l+1))
				out += rb2[l] * hup[k];
			rb[indx] = out;
			break;
		case 54:
			indx2 = mod(indx2 - 1);
			rb2[indx2] = 2 * in;
			out = 0;
			for(k = 0, l = indx2; k <= flengup; k += intrate, l = mod(l+1))
				out += rb2[l] * hup[k];
			rb[indx] = out;
			break;
		default:
			rb[indx] = in;
	}
}

void firin0()
{
	double	out;
	int	k, l;

	out = 0;
	for(k = 1, l = indx2 ; k <= flengup; k += intrate, l = mod(l+1))
		out += rb2[l] * hup[k];

	indx = mod(indx - 1);
	rb[indx] = out;
}

double firout(os)
int	os;
{
	double	out;
	int	k, l;

	out = 0;
	for(k = os, l = indx ; k <= flengdn; k += intrate, l = mod(l + 1))
		out += rb[l] * hdn[k];

	return(out);
}

void firinit()
{
	FILE	*fp, *fopen();

	if((fp = fopen(coef, "r")) == NULL) {
		fprintf(stderr, "%s: cannot open %s\n", cmnd, coef);
		exit(1);
	}
	flengdn = freada(hdn, RBSIZE + 1, fp);
	fclose(fp);
	if(--flengdn < 0) {
		fprintf(stderr, "%s: cannot read filter coefficients\n", cmnd);
		exit(1);
	}
	if(type == 52 || type == 54){
		if((fp = fopen(coef1, "r")) == NULL) {
			fprintf(stderr, "%s: cannot open %s\n", cmnd, coef1);
			exit(1);
		}
		flengup = freada(hup, RBSIZE +1, fp);
		fclose(fp);
		if(--flengup < 0) {
			fprintf(stderr, "%s: cannot read filter coefficients\n",cmnd);
			exit(1);
		}
		start = (((flengup / 2) * intrate) + (flengdn / 2)) / decrate;
	}
	else
		start = flengdn / (2 * decrate);
}

int freada( double *p, int bl, FILE *fp)
{
	int c;
	char buf[BUFLENGTH];

	c = 0;
	while( c < bl ){
		if( fgets( buf, BUFLENGTH, fp) == NULL) break;
		p[c] = atof( buf);
		c+=1;
	}
	return c;
}
