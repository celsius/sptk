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
*    Calculation of Root Mean Squared Error				*
*									*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		rmse [ options ] [ infile ] > stdout			*
*	options:							*
*		-l l     :  frame length	[entire data]		*
*			    if l>0, calculate rmse frame by frame	*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ..., x(l-1),				*
*	stdout:								*
*		rmse							*
*		    , x(0)+x(1)...+x(l-1)/l				*
*	require:							*
*		rmse()							*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Required Functions  */
double	rmse(), sqrt();


/*  Default Values  */
#define LENG		0


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - calculation of root mean squared error\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "      -l l  : frame length       [%d]\n", LENG);
    fprintf(stderr, "      -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "      data sequence (float)      [stdin]\n");
    fprintf(stderr, "  file1:\n");
    fprintf(stderr, "      data sequence (float)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "      root mean squared error (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "      if l > 0, calculate rmse frame by frame\n");
    fprintf(stderr, "\n");
    exit(status);
}

void main(int argc, char **argv)
{
    int		l = LENG, num = 0;
    FILE	*fp = stdin, *fp1 = NULL;
    double	*x, *y, x1, y1, sub, z = 0.0;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
        else if (fp1 == NULL)
            fp1 = getfp(*argv, "r");
        else
            fp = getfp(*argv, "r");

    if(l > 0) {
	x = dgetmem(l+l);
	y = x + l;
	while (freadf(x, sizeof(*x), l, fp) == l &&
	       freadf(y, sizeof(*y), l, fp1)== l ){
	    z = rmse(x, y, l);
	    fwritef(&z, sizeof(z), 1, stdout);
	}
    }
    else {
	while (freadf(&x1, sizeof(x1), 1, fp) == 1 &&
	       freadf(&y1, sizeof(y1), 1, fp1)== 1){
	    sub = x1 - y1;
	    z += sub * sub;

	    num++;
	}
	z = sqrt(z /= num);
	fwritef(&z, sizeof(z), 1, stdout);
    }
    exit(0);
}

