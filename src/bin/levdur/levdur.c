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
*    Solve an Autocorrelation Normal Equation				*
*			Using Levinson-Durbin Method			*
*									*
*					1987.9  K.Tokuda		*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		levdur [ options ] [ infile ] > stdout			*
*	options:							*
*		-m m     :  order of correlation	[25]		*
*	infile:								*
*		autocorrelation						*
*		    , r(0), r(1), ..., r(m),				*
*	stdout:								*
*		LP Coefficeints						*
*		    , K, a(1), ..., a(m),				*
*	require:							*
*		levdur()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: levdur.c,v 1.5 2006/12/19 13:04:21 mr_alex Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>

/*  Default Values  */
#define ORDER		25


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - solve an autocorrelation normal equation\n",cmnd);
    fprintf(stderr, "                    using Levinson-Durbin method\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -m m  : order of correlation [%d]\n", ORDER);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       autocorrelation (float)      [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       LP coefficients (float)\n");
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}


int main(int argc, char **argv)
{
    int		m = ORDER, flag;
    FILE	*fp = stdin;
    double	*r, *a;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'm':
		    m = atoi(*++argv);
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

    a = dgetmem(m+m+2);
    r = a + m + 1;

    while (freadf(r, sizeof(*r), m+1, fp) == m+1){
	flag = levdur(r, a, m, -1.0);
	fwritef(a, sizeof(*a), m+1, stdout);
    }
    exit(0);
}

