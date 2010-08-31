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
/*                1996-2010  Nagoya Institute of Technology          */
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

/*********************************************************************

    $Id: _vstat.c,v 1.1 2010/08/31 10:45:23 mataki Exp $

    Calculation of the percentile of t-distribution.

**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

double norm_percent(const double alpha)
{
   int i;
   double a, x, u;
   static double b[11] = {
      1.570796288,      0.03706987906,    -0.8364353589e-3,
      -0.2250947176e-3, 0.6841218299e-5,  0.5824238515e-5,
      -0.104527497e-5,  0.8360937017e-7,  -0.3231081277e-8,
      0.3657763036e-10, 0.6936233982e-12
   };

   if (alpha == 0.5) {
      return 0.0;
   }

   a = alpha;
   if (a > 0.5) {
      a = 1.0 - a;
   }
   x = -log(4.0 * a * (1.0 - a));
   u = b[0];
   for (i = 1; i < 11; i++) {
      u += (b[i] * pow(x, (double)i));
   }
   if (alpha > 0.5) {
      return -sqrt(u * x);
   }
   else {
      return sqrt(u * x);
   }
}

double t_lower(const double t, const int df)
{
   int i;
   double c2, p, s;

   c2 = df / (df + t * t);
   s = sqrt(1 - c2);
   if (t < 0) {
      s = -s;
   }
   p = 0;
   for (i = df % 2 + 2; i <= df; i += 2) {
      p += s;
      s *= (i - 1) * c2 / i;
   }
   if (df & 1) {
      return (0.5 + (p * sqrt(c2) + atan(t / sqrt(df))) / PI);
   }
   else {
      return ((1 + p) / 2);
   }
}

double t_percent(const double p, const int df)
{
   double f, y1, y2, y3, y4, y5, q, r, u, u2, x, delta;
   int n;

   q = (1.0 - p) / 2;
   f = (double)df;

   u = norm_percent(q);
   u2 = u * u;

   y1 = (u2 + 1) / 4;
   y2 = ((5 * u2 + 16) * u2 + 3) / 96;
   y3 = (((3 * u2 + 19) * u2 + 17) * u2 - 15) / 384;
   y4 = ((((79 * u2 + 776) * u2 + 1482) * u2 - 1920) * u2 - 945) / 92160;
   y5 = (((((27 * u2 + 339) * u2 + 930) * u2 - 1782) * u2 - 765) * u2 + 17955) / 368640;

   x = u * (1 + (y1 + (y2 + (y3 + (y4 + y5 / f) / f) / f) / f) / f);

   if (df <= pow(log10(1 - q), 2) + 3) {
      r = t_lower(x, df);
      n = df + 1;
      delta = (r - (1 - q))
         / exp((n * log((double)n / (df + x * x))
                + log((double)df / n / 2 / PI) - 1
                + (1.0 / n - 1.0 / df) / 6.0) / 2.0);
      x -= delta;
   }

   return (x);
}
