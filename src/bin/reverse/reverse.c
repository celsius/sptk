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

