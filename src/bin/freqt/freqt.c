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
*    Frequency Transformation						*
*									*
*					1988.2  K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		freqt [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of minimum phase sequence 	[25]	*
*		-M M     :  order of warped sequence		[25]	*
*		-a a     :  all-pass constant of input sequence	[0]	*
*		-A A     :  all-pass constant of output sequence[0.35]	*
*	infile:								*
*		minimum phase sequence					*
*		    , c(0), c(1), ..., c(M),				*
*	stdout:								*
*		warped sequence						*
*		    , c~(0), c~(1), ..., c~(N),				*
*	require:							*
*		freqt()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>

/*  Default Values  */
#define ORDERC1		25
#define ORDERC2		25
#define	ALPHA1		0.0
#define	ALPHA2		0.35


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - frequency transformation\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of minimum phase sequence      [%d]\n", ORDERC1);
    fprintf(stderr, "       -M M  : order of warped sequence             [%d]\n", ORDERC2);
    fprintf(stderr, "       -a a  : all-pass constant of input sequence  [%g]\n", ALPHA1);
    fprintf(stderr, "       -A A  : all-pass constant of output sequence [%g]\n", ALPHA2);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       minimum phase sequence (float)               [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       warped sequence (float)\n");
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif	    
    fprintf(stderr, "\n");
    exit(status);
}

int main(int argc, char **argv)
{
    int		m1 = ORDERC1, m2 = ORDERC2;
    FILE	*fp = stdin;
    double	*c1, *c2, a1 = ALPHA1, a2 = ALPHA2, a, atof();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'm':
		    m1 = atoi(*++argv);
		    --argc;
		    break;
		case 'M':
		    m2 = atoi(*++argv);
		    --argc;
		    break;
		case 'a':
		    a1 = atof(*++argv);
		    --argc;
		    break;
		case 'A':
		    a2 = atof(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");

    c1 = dgetmem(m1+m2+2);
    c2 = c1 + m1 + 1;

    a = (a2 - a1) / (1 - a1*a2);

    while (freadf(c1, sizeof(*c1), m1+1, fp) == m1+1){
	freqt(c1, m1, c2, m2, a);
	fwritef(c2, sizeof(*c2), m2+1, stdout);
    }
    exit(0);
}
