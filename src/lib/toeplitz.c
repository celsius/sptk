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
    $Id: toeplitz.c,v 1.3 2006/12/11 06:59:51 mr_alex Exp $

    Solve a Synmetric Toeplitz Set of Linear Equations	

	int toeplitz(t, a, b, n, eps)	< Ta=b >

	double *t  : Toeplitz elements -> T(i,j) = t(|i-j|) t[0]..t[n-1]
	double *a  : solution vector of equation
	double *b  : known vector
	int    n   : system order
	double eps : singular check (eps(if -1., 1.0e-6 is assumed))

	return value : 	0  -> normally completed
			-1 -> abnormally completed

****************************************************************/

#include <stdio.h>
#include <SPTK.h>

int toeplitz(double *t, double *a, double *b, int n, double eps)
{
    register int 	l, k;
    static double 	*c = NULL, *cc;
    static int		size;
    double 	  	rmd, mue, mue2;
    
    if (c == NULL){
	c = dgetmem(n+n+2);
	cc = c + n;
	size = n;
    }
    if (n > size){
	free(c);
	c = dgetmem(n+n+2);
	cc = c + n;
	size = n;
    }
    
    if (eps < 0.0) eps = 1.0e-6;

    fillz(c, sizeof(*c), n+1);

    rmd = t[0];
    if (((rmd < 0.0) ? -rmd : rmd) <= eps) return(-1);
    
    a[0] = b[0] / rmd;

    for (l=1; l<n; l++){
	mue = -t[l];
	for (k=1; k<l; k++)
	    mue -= c[k] * t[l-k];
	mue /= rmd;

	for (k=1; k<l; k++)
	    cc[k] = c[k] + mue * c[l-k];
	cc[l] = mue;

	rmd = (1.0 - mue*mue) * rmd;
	if (((rmd < 0.0) ? -rmd : rmd) <= eps) return(-1);

	for (k=1; k<=l; k++) c[k] = cc[k];

	mue2 = b[l];
	for (k=0; k<=l-1; k++)
	    mue2 += c[l-k] * b[k];
	mue2 /= rmd;

	for (k=0; k<l; k++)
	    a[k] += mue2 * c[l-k];
	a[l] = mue2;
    }
    return(0);
}
