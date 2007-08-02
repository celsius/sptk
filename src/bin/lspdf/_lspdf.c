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

/****************************************************************

    $Id: _lspdf.c,v 1.5 2007/08/02 08:35:12 heigazen Exp $

    LSP Speech Synthesis Digital Filter

	double	lspdf_even(x, a, m, d)

	double	x   : input
	double	*f  : LSP coefficients
	int	m   : order of coefficients
	double  *d  : delay

	return value : filtered data

*****************************************************************/
#include <stdio.h>

double	lspdf_even(double x, double *f, int m, double *d)
{
    double  	  *d1, *d2, *lsp, x1, x2;
    int  i;

    d1 = d + 1;
    d2 = d1 + m;
    lsp = f + 1;
    x1 = x2 = d[0];

    for(i=0; i<m; i+=2){
	d1[i] -= 2.0 * x1 * cos(lsp[i]);
	d2[i] -= 2.0 * x2 * cos(lsp[i+1]);
	d1[i+1] += x1;
	d2[i+1] += x2;
	x += d1[i] + d2[i];
	x1 = d1[i+1];
	x2 = d2[i+1];
    }
    
    x -= d2[m-1] - d1[m-1];

    for(i=m-1; i>0; i--){
	d1[i] = d1[i-1];
	d2[i] = d2[i-1];
    }
    d1[0] = d2[0] = d[0];
    d[0] = -0.5 * x;

    return(x);
}

double	lspdf_odd(double x, double *f, int m, double *d)
{
    int  i;
    int           mh1, mh2;
    double  	  *d1, *d2, *lsp, x1, x2;

    mh1 = (m+1) / 2;
    mh2 = (m-1) / 2;

    d1 = d + 1;
    d2 = d1 + (mh1+mh1-1);
    lsp = f + 1;
    x1 = x2 = d[0];

    for(i=0; i<m-1; i+=2){
	d1[i] -= 2.0 * x1 * cos(lsp[i]);
	d2[i] -= 2.0 * x2 * cos(lsp[i+1]);
	d1[i+1] += x1;
	d2[i+1] += x2;
	x += d1[i] + d2[i];
	x1 = d1[i+1];
	x2 = d2[i+1];
    }
    d1[i] -= 2.0 * x1 * cos(lsp[i]);
    x += d1[i] - d2[i];

    for(i=m-1; i>0; i--){
	d1[i] = d1[i-1];
	d2[i] = d2[i-1];
    }
    d1[0] = d2[0] = d[0];
    d[0] = -0.5 * x;

    return(x);
}

