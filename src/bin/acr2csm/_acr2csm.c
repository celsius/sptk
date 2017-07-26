/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2017  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/***************************************************************

    $Id$

    Transformation Autocorrelation to CSM

       void acr2csm(r, csm, m)

       double  *r   : Autocorrelation coefficients
       double  *csm : CSM parameters
       int      m   : order of autocorrelation

****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

void quicksort(double *x, int left, int right)
{
   int i, j;
   double pivot;
   double tmp;

   i = left;
   j = right;

   pivot = x[(left + right) / 2];

   while (1) {

      while (x[i] > pivot)
         i++;

      while (pivot > x[j])
         j--;
      if (i >= j)
         break;

      tmp = x[i];
      x[i] = x[j];
      x[j] = tmp;

      i++;
      j--;
   }

   if (left < i - 1)
      quicksort(x, left, i - 1);
   if (j + 1 < right)
      quicksort(x, j + 1, right);
}

int vander(double *c, double *a, double *b, const int n, double eps) {
   int i, j;
   double e, numer, denom;
   static double *d = NULL;
   static int size;

   if (d == NULL) {
      d = dgetmem(n);
      size = n;
   }

   if (n > size) {
      free(d);
      d = dgetmem(n);
      size = n;
   }

   if (eps < 0.0)
      eps = 1.0e-6;

   fillz(d, sizeof(*d), n);
   for (j = 0; j < n; j++) {
      for (i = n - j - 1; i < n - 1; i++) {
         d[i] += -c[j] * d[i + 1];
      }
      d[n - 1] += -c[j];
   }

   for (j = 0; j < n; j++) {
      e = 1.0;
      numer = b[n - 1];
      denom = 1.0;
      for (i = n - 2; i >= 0; i--) {
         e = d[i + 1] + c[j] * e;
         numer = numer + b[i] * e;
         denom = denom * c[j] + e;
      }

      if (fabs(denom) <= eps)
         return -1;

      a[j] = numer / denom;
   }

   return 0;
}

unsigned long long nck(int n, int k)
{
   int i;
   unsigned long long p = 1;

   if (2 * k > n)
      k = n - k;
   for (i = 1; i <= k; i++) {
      p *= n--;
      p /= i;
   }

   return p;
}

int acr2csm(double *r, double *csm, const int m)
{
   int i, k, l, n = (m + 1) / 2;
   double sum, tmp;
   static complex *z = NULL;
   static double *u = NULL, *h, *p, *x, *q;
   static int size;

   if (u == NULL || x == NULL) {
      u = dgetmem(m + 1 + n * n + n + 1 + n + n);
      h = u + m + 1;
      p = h + n * n;
      x = p + n + 1;
      q = x + n;
      z = cplx_getmem(n + 1);
      size = m;
   }

   if (m > size) {
      free(u);
      free(z);
      u = dgetmem(m + 1 + n * n + n + 1 + n + n);
      h = u + m + 1;
      p = h + n * n;
      x = p + n + 1;
      q = x + n;
      z = cplx_getmem(n + 1);
      size = m;
   }

   for (l = 0; l <= m; l++) {
      sum = 0.0;
      for (k = 0; k <= l; k++) {
         sum += nck(l, k) * r[abs(2 * k - l)];
      }
      u[l] = sum / pow(2.0, l);
   }

   for (i = 0; i < n * n; i++)
      h[i] = u[i % n + i / n];

   /* solve a Hankel system */
   if (cholesky(h, p, &(u[n]), n, 1.0e-6) == -1)
      return -1;

   for (i = 0; i < n; i++) {
      p[i] *= -1.0;
   }
   p[n] = 1.0;
   for (i = 0; i < (n + 1) / 2; i++) {
      tmp = p[i];
      p[i] = p[n - i];
      p[n - i] = tmp;
   }

   /* solve roots of a polynomial equation */
   root_pol(p, n, z, 1, 1e-12, 1000);

   for (i = 1; i <= n; i++)
      x[i - 1] = z[i].re;
   quicksort(x, 0, n - 1);

   /* save CSM frequencies */
   for (i = 0; i < n; i++) {
      if (fabs(x[i]) > 1.0)
         return -1;

      csm[i] = acos(x[i]);
   }

   /* solve a Van der Monde system */
   if (vander(x, q, &(u[0]), n, 0.0) == -1)
      return -1;

   /* save CSM intensities */
   for (i = 0; i < n; i++)
      csm[i + n] = q[i];

   return 0;
}
