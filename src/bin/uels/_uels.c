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

    $Id: _uels.c,v 1.10 2007/08/07 05:05:39 heigazen Exp $

    Unbiased Estimation of Log Spectrum

 int uels(xw, flng, c, m, itr1, itr2, dd, e);

 double   *xw  : input sequence
 int      flng : frame length
 double   *c   : cepstrum
 int      m    : order of cepstrum
 int      itr1 : minimum number of iteration
 int      itr2 : maximum number of iteration
 double   dd   : end condition
 double   e    : initial value for log-periodgram

 return value :  0 -> completed by end condition
   -1-> completed by maximum iteration

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>

int uels (double *xw, const int flng, double *c, const int m, const int itr1, const int itr2, const double dd, const double e)
{
   int i, j, flag=0;
   double k;
   static double *x=NULL, *r, *cr, *y, *a;
   static int size_x, size_a;

   if (x==NULL) {
      x = dgetmem(4*flng);
      a = dgetmem(m+1);
      r = x + flng;
      cr = r + flng;
      y = cr + flng;
      size_x = flng;
      size_a = m;
   }
   if (flng>size_x) {
      free(x);
      x = dgetmem(4*flng);
      r = x + flng;
      cr = r + flng;
      y = cr + flng;
      size_x = flng;
   }
   if (m>size_a) {
      free(a);
      a = dgetmem(m+1);
      size_a = m;
   }

   movem(xw, x, sizeof(*xw), flng);

   fftr(x, y, flng);    /*  x+jy : X(w)  */
   for (i=0; i<flng; i++) {  /*  x : log|X(w)|^2  */
      x[i] = x[i]*x[i] + y[i]*y[i] + e;
      if (x[i] <= 0) {
         fprintf(stderr,"uels : The log periodogram has the value of '0' consider to use '-e' option !\n");
         exit(0);
      }
      x[i] = cr[i] = log(x[i]);
   }
   ifftr(cr, y, flng);    /*  cr : c(m)  */
   
   /*  initial value  */
   k = exp(cr[0]);
   for (i=1; i<=m; i++) c[i] = cr[i];

   for (j=1; j<=itr2; j++) {
      cr[0] = 0.0;

      for (i=1; i<=m;   i++) cr[i] = c[i];
      for (   ; i<flng; i++) cr[i] = 0.0;

      fftr(cr, y, flng);   /*  cr+jy : log D(z)  */
      for (i=0; i<flng; i++)
         r[i] = exp(x[i] - cr[i] - cr[i]);
      ifftr(r, y, flng);   /*  r : autocorr  */

      c[0] = k;
      k = r[0];

      if (j >= itr1) {
         if (fabs((k - c[0])/c[0])<dd) {
            flag = 1;
            break;
         }
         k = c[0];
      }

      lplp(r, a, m);
      for (i=1; i<=m; i++) c[i] -= a[i];
   }

   c[0] = 0.5 * log(k);
   if (flag) return(0);
   else     return(-1);
}

/* Fast Algorithm for Linear Prediction with Linear Phase */
void lplp (double *r, double *c, const int m)
{
   int k, n;
   double pn, alpha, beta, gamma, tz=r[0]/2, rtz=1/tz, to=r[1], rttz=0, tto=1;
   static double *p=NULL, *pp;
   static int size;

   if (p==NULL) {
      p = dgetmem(m+m+4);
      pp = p + m + 2;
      size = m;
   }
   if (m>size) {
      free(p);
      p = dgetmem(m+m+2);
      pp = p + m + 1;
      size = m;
   }

   c[0] = 1.0 / r[0];
   p[0] = 1.0;
   p[1] = 0.0;
   pp[0] = 0.0;

   for (k=1; k<=m; k++) {
      p[k+1] = 0.0;
      pp[k] = 0.0;
      beta = -tz * rttz;
      alpha = tto * rttz;
      alpha -= (tto = to) * (rttz = rtz);
      pn = p[1] + p[1] + alpha * p[0] + beta * pp[0];
      pp[0] = p[0];
      p[0] = pn;

      for (n=1; n<=k; n++) {
         pn = p[n+1] + pp[n - 1] + alpha * p[n] + beta * pp[n];
         pp[n] = p[n];
         p[n] = pn;
      }
      for (n=1,tz=p[0]*r[k]; n<=k; n++)
         tz += p[n] * (r[k-n] + r[k+n]);

      for (n=1,to=p[0]*r[1+k]; n<=k; n++)
         to += p[n] * (r[1+k-n]+r[1+k+n]);

      gamma = 0.5 * p[0] * (rtz = 1 / tz);

      for (n=0; n<k; n++)
         c[n] = c[n] + gamma * p[n];
      c[k] = gamma * p[k];
   }
   for (c[0]=1.0/c[0], n=1; n<=m; n++) c[n] *= c[0];
   
   return;
}

