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

   $Id$

   Mel-Generalized Cepstral Analysis

   int mgcep(xw, flng, b, m, a, g, itr1, itr2, dd, e);

   double   *xw  : input sequence
   int      flng : frame length
   double   *b   : coefficient b'(m)
   int      m    : order of mel cepstrum
   double   a    : alpha
   double   g    : gamma
   int      n    : order of recursions
   int      itr1 : minimum number of iteration
   int      itr2 : maximum number of iteration
   double   dd   : end condition
   double   e    : initial value for log-periodgram

   return value :    0 -> completed by end condition
                     -1-> completed by maximum iteration

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <SPTK.h>

double gain(double *er, double *c, int m, double g);
void b2c(double *b, int m1, double *c, int m2, double a);
void ptrans(double *p, register int m, double a);
void qtrans(double *q, register int m, double a);

int mgcep(double *xw, int flng, double *b, int m, double a, double g, int n, int itr1, int itr2, double dd, double e)
{
   int i, j, flag = 0;
   static double *x = NULL, *y, *d;
   static int size_x, size_c;
   double ep, epo, newton();
   
   if (x == NULL) {
      x = dgetmem(flng+flng);
      y = x + flng;
      size_x = flng;
      d = dgetmem(m+1);
      size_c = m;
   }
   if (flng>size_x) {
      free(x);
      x = dgetmem(flng+flng);
      y = x + flng;
      size_x = flng;
   }
   if (m>size_c) {
      free(d);
      d = dgetmem(m+1);
      size_c = m;
   }
   
   movem(xw, x, sizeof(*x), flng);
   
   /*  periodegram  */
   fftr(x, y, flng);
   for (i=0; i<flng; i++)
      x[i] = x[i]*x[i] + y[i]*y[i] + e;

   /* initial value */    
   fillz(b, sizeof(*b), m+1);
   ep = newton(x, flng, b, m, a, -1.0, n, 0);

   if (g != -1.0) {
      if (a != 0.0) {
         ignorm(b, b, m, -1.0);   /*  K, b'r(m)    -> br(m)         */
         b2mc(b, b, m, a);        /*  br(m)        -> c~r(m)        */
         gnorm(b, d, m, -1.0);    /*  c~r(m)       -> K~, c~'r(m)   */
      }
      else 
         movem(b, d, sizeof(*b), m+1);

      gc2gc(d, m, -1.0, b, m, g);   /*  K~, c~'r(m)  -> K~, c~'r'(m)  */

      if (a != 0.0) {
         ignorm(b, b, m, g);    /*  K~, c~'r'(m) -> c~r(m)        */
         mc2b(b, b, m, a);      /*  c~r(m)       -> br(m)         */
         gnorm(b, b, m, g);     /*  br(m)        -> K, b'r'(m)    */
      }
   }
   
   /*  Newton-Raphson method  */
   if (g != -1.0) {
      for (j=1; j<=itr2; j++) {
         epo = ep;
         ep = newton(x, flng, b, m, a, g, n, i);

         if (j >= itr1)
            if (fabs((epo - ep)/ep) < dd) {
               flag = 1;
               break;
            }
      }
   }

   if (flag) return(0);
   else      return(-1);
}

double newton(double *x, int flng, double *c, int m, double a, double g, int n, int j)
{
   int i, m2;
   double t, s, tr, ti, trr, tii, log();
   static double  *cr = NULL, *ci, *pr, *qr, *qi, *rr, *ri, *b;
   static int   size_cr, size_b;
   
   if (cr == NULL) {
      cr = dgetmem(7*flng);
      ci = cr + flng;  pr = ci + flng;
      qr = pr + flng;  qi = qr + flng;
      rr = qi + flng;  ri = rr + flng;
      size_cr = flng;

      b = dgetmem(m+1);
      size_b = m;
   }
   if (flng > size_cr) {
      free(cr);
      cr = dgetmem(7*flng);
      ci = cr + flng;  pr = ci + flng;
      qr = pr + flng;  qi = qr + flng;
      rr = qi + flng;  ri = rr + flng;
      size_cr = flng;
   }
   if (m > size_b) {
      free(b);
      b = dgetmem(m+1);
      size_b = m;
   }

   m2 = m + m;
   
   fillz(cr, sizeof(*cr), flng);
   movem(&c[1], &cr[1], sizeof(*c), m);
   
   if (a != 0.0) 
   b2c(cr, m, cr, n, -a);
   
   fftr(cr, ci, flng);         /* cr +j ci : FFT[c]  */

   if (g == -1.0) 
      movem(x, pr, sizeof(*x), flng);
   else if (g == 0.0)
      for (i=0; i<flng; i++) 
         pr[i] = x[i] / exp(cr[i] + cr[i]);
   else
       for (i=0; i<flng; i++) {
          tr = 1 + g * cr[i]; ti = g * ci[i];
          s = (trr = tr * tr) + (tii = ti * ti);
          t = x[i] * pow(s, -1.0/g);
          pr[i] = (t /= s);
          rr[i] = tr * t; ri[i] = ti * t;
          t /= s; qr[i] = (trr - tii) * t;
          s = tr * ti * t; qi[i] = s + s;
       }

   ifftr(pr, ci, flng);

   if (a!=0.0) 
      b2c(pr, n, pr, m2, a);
   
   if (g==0.0 || g==-1.0) {
      movem(pr, qr, sizeof(*pr), m2+1);
      movem(pr, rr, sizeof(*pr), m+1);
   }
   else {
      ifft(qr, qi, flng);
      ifft(rr, ri, flng);
      
      if (a!=0.0) {
         b2c(qr, n, qr, n, a);
         b2c(rr, n, rr, m, a);
      }
   }

   if (a!=0.0) {
      ptrans(pr, m, a);
      qtrans(qr, m, a);
   }

   /*  c[0] : gain, t : epsilon  */
   if (g!=-1.0) 
      c[0] = sqrt(t = gain(rr, c, m, g));
   
   if (g==-1.0)
      fillz(qr, sizeof(*qr), m2+1);
   else if (g!=0.0) 
      for (i=2; i<=m2; i++) 
         qr[i] *= 1.0 + g;
   
   if (theq(pr, &qr[2], &b[1], &rr[1], m, -1.)) {
      fprintf(stderr, "mgcep : Error in theq() at %dth iteration !\n",j);
      exit(1);
   }

   for (i=1; i<=m; i++) c[i] += b[i];

   /*  c[0] : gain, t : epsilon  */
   if (g==-1.0) 
      c[0] = sqrt(t = gain(rr, c, m, g));    
   
   return(log(t));
}

/*  gain(epsilon) calculation  */
double gain(double *er, double *c, int m, double g)
{
   int i;
   double t;

   if (g!=0.0) {
      for (t=0.0,i=1; i<=m; i++) t += er[i] * c[i];
      return(er[0] + g*t);
   }
   else return(er[0]);
}

/*  b'(m) to c(m)  */
void b2c(double *b, int m1, double *c, int m2, double a)
{
   int i, j;
   static double *d = NULL, *g;
   static int size;
   double k;
   
   if (d == NULL) {
      size = m2;
      d = dgetmem(size+size+2);
      g = d + size + 1;
   }
   if (m2 > size) {
      free(d);
      size = m2;
      d = dgetmem(size+size+2);
      g = d + size + 1;
   }

   k = 1 - a * a;

   fillz(g, sizeof(*g), m2 + 1);
   
   for (i=-m1; i<=0; i++) {
      d[0] = g[0];
      g[0] = b[-i];

      if (1<=m2) 
         g[1] = k * d[0] + a * (d[1] = g[1]);

      for (j=2; j<=m2; j++)
         g[j] = d[j - 1] + a * ((d[j] = g[j]) - g[j - 1]);
   }
   movem(g, c, sizeof(*g), m2 + 1);
}

/*  recursion for p(m)  */
void ptrans(double *p, register int m, double a)
{
   double d, o;

   d = p[m];
   for (m--; m > 0; m--) { 
      o = p[m] + a * d;
      d = p[m]; p[m] = o;
   }
   o =  a * d; 
   p[m] = (1. - a * a) * p[m] + o + o;
}

/*  recursion for q(m)  */
void qtrans(double *q, register int m, double a)
{
   int i;
   double d, o;

   m += m;
   i = 1; d = q[i];
   for (i++; i <= m; i++) { 
      o = q[i] + a * d;
      d = q[i]; q[i] = o;
   }
}

