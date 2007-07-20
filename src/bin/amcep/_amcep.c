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

    $Id: _amcep.c,v 1.6 2007/07/20 07:58:28 heigazen Exp $

    Adaptive Mel Cepstral Analysis

    double amcep(x, b, m, a, lambda, step, tau, pd, eps);

    double x      : input sequence
    double *b     : MLSA filter coefficients
    int    m      : order of cepstrum
    double a      : alpha
    double lambda : leakage factor
    double step   : step size
    double tau    : momentum constant
    int    pd     : order of pade approximation
    double eps    : minimum value for epsilon

    return value  : prediction error

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>

double amcep (double x, double *b, const int m, const double a, const double lambda, const double step, const double tau, const int pd, const double eps)
{
   int i;
   static double *bb=NULL, *d, *ep, *e, xx, gg=1.0;
   static int size;
   double mu, tx;
    
   if (bb==NULL) {
      bb = dgetmem(3*(m+1)+3*(pd+1)+pd*(m+2));
      e  = bb + m + 1;
      ep = e  + m + 1;
      d  = ep + m + 1;
      size = m;
   }
   if (m>size) {
      free(bb);
      bb = dgetmem(3*(m+1)+3*(pd+1)+pd*(m+2));
      e  = bb + m + 1;
      ep = e  + m + 1;
      d  = ep + m + 1;
      size = m;
   }
	
   for (i=1; i<=m; i++)
      bb[i] = -b[i];
    
   x = mlsadf(x, bb, m, a, pd, d);
   phidf(xx, m, a, e);
   xx = x;

   gg = gg * lambda + (1.0 - lambda) * x * x;
   gg = (gg<eps) ? eps : gg;
   b[0] = 0.5 * log(gg);

   mu = step / (double) m / gg;
   tx = 2 * (1.0 - tau) * x;
    
   for (i=1; i<=m; i++) {
      ep[i] = tau * ep[i] - tx * e[i];
      b[i] -= mu * ep[i];
   }
    
   return(x);
}

void phidf (const double x, const int m, double a, double *d)
{
   int i;

   d[0] = a * d[0] + (1.0 - a*a) * x;    
   for (i=1; i<m; i++)
      d[i] += a * (d[i+1] - d[i-1]);

   for (i=m; i>=1; i--) 
      d[i] = d[i-1];
}
