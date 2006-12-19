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
*    Digital Filter in Standard Form					*
*									*
*					1989.6   K.Tokuda		*
*					1995.12  N.Isshiiki modified	*
*	usage:								*
*		dfs [ options ] [ infile ] > stdout			*
*	options:							*
*		-a k a1 a2 ... aM  : denominator coefficients (float)	*
*					k:gain				*
*		-b b0 b1 b2 ... bM : numerator coefficients (flaot)	*
*		-p file_p          : denominator coefficients (float)	*
*		-z file_z          : numerator coefficients (float)	*
*	infile:								*
*		input (float)						*
*	stdout:								*
*		output (float)						*
*	note:								*
*		M, N <= 2047						*
************************************************************************/

static char *rcs_id = "$Id: dfs.c,v 1.6 2006/12/19 13:02:44 mr_alex Exp $";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <SPTK.h>
#include <string.h>

/* Default Values */
#define SIZE 2048


/* Command Name */
char *cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - digital filter in standard form\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout \n", cmnd);
	fprintf(stderr, "  options:\n");           
	fprintf(stderr, "       -a K  a1...aM : denominator coefficients      [N/A]\n");
	fprintf(stderr, "       -b b0 b1...bN : numerator coefficients        [N/A]\n");
	fprintf(stderr, "       -p pfile      : denominator coefficients file [NULL]\n");
	fprintf(stderr, "       -z zfile      : numerator coefficients file   [NULL]\n");
	fprintf(stderr, "       -h            : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       filter input (float)                          [stdin]\n");	
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       filter output (float)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       M,N <= %d \n",SIZE-1);
#ifdef SPTK_VERSION
	fprintf(stderr, "\n");
	fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif
	fprintf(stderr, "\n");
	exit(status);
}

int main(int argc, char *argv[])
{
	int i;
	static double a[SIZE], b[SIZE];
	static double d[SIZE];
	int bufp = 0;
	int na = -1, nb = -1;
	double x;
	char *file_z = "", *file_p = "";
	FILE *fp_z, *fp_p;
	double atof();

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
	else
		cmnd++;
	while (--argc)
		if (**++argv == '-')
			switch (*(*argv+1)) {
			case 'a':
				i = 0;
				while (argc  - 1 &&
				    ! isalpha(*(*(argv + 1) + 1))   ){
					a[i++] = atof(*++argv);
					argc--;
					na++;
				}
				break;
			case 'b':
				i = 0;
				while (argc  - 1 &&
				    ! isalpha(*(*(argv + 1) + 1))   ){
					b[i++] = atof(*++argv);
					argc--;
					nb++;
				}
				break;
			case 'z':
				argc--;
				file_z = *++argv;
				break;
			case 'p':
				argc--;
				file_p = *++argv;	
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr, "%s : invalid option !\n"
 					, cmnd);
				usage(1);
			}
		else {
			fprintf(stderr, "%s : invalid option !\n", cmnd);
			usage(1);
		}

	if (*file_z != '\0'){
		fp_z = getfp(file_z, "r");
		nb = freadf(b, sizeof(*b), SIZE, fp_z) - 1;
	}
	if (*file_p != '\0'){
		fp_p = getfp(file_p, "r");
		na = freadf(a, sizeof(*a), SIZE, fp_p) - 1;
	}

	if (na == -1){
		na = 0;
		a[0] = 1.0;
	}
	if (nb == -1){
		nb = 0;
		b[0] = 1.0;
	}

	while(freadf(&x, sizeof(x), 1, stdin) == 1){
		x = dfs(x, a, na, b, nb, d, &bufp);
		fwritef(&x, sizeof(x), 1, stdout);
	}
	exit(0);
}
