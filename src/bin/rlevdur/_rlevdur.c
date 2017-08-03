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

/****************************************************************

    $Id$

    Solve an Autocorrelation Normal Equation
    Using Reverse Levinson-Durbin Method

       int rlevdur(a, r, m, eps);

       double  *a    : LP coefficients
       double  *r    : autocorrelation sequence
       int     m     : order of LPC
       double  eps   : singular check

       return  value :  0  -> normally completed
                        -1 -> abnormally completed

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

int rlevdur(double *a, double *r, const int m, double eps)
{
   int i, j;
   double rmd, sum;
   static double **u = NULL, *e = NULL;
   static int size;

   if (u == NULL && e == NULL) {
      u = ddgetmem(m + 1, m + 1);
      e = dgetmem(m + 1);
      size = m;
   }

   if (m > size) {
      free(u);
      free(e);
      u = ddgetmem(m + 1, m + 1);
      e = dgetmem(m + 1);
      size = m;
   }

   for (j = 0; j <= m; j++) {
      u[j][j] = 1.0;
   }

   for (j = 0; j < m; j++) {
      u[m][j] = a[m - j];
   }
   e[m] = a[0] * a[0];

   for (i = m - 1; i > 0; i--) {
      rmd = (1.0 - u[i + 1][0] * u[i + 1][0]);
      if ((rmd < 0.0) ? -rmd : rmd <= eps)
         return (-1);
      for (j = 0; j < i; j++) {
         u[i][i - j - 1] =
             (u[i + 1][i - j] - u[i + 1][0] * u[i + 1][j + 1]) / rmd;
      }
      e[i] = e[i + 1] / rmd;
   }
   e[0] = e[1] / (1.0 - u[1][0] * u[1][0]);

   r[0] = e[0];
   for (i = 1; i <= m; i++) {
      sum = 0.0;
      for (j = 1; j < i; j++) {
         sum -= u[i - 1][i - j - 1] * r[i - j];
      }
      r[i] = sum - u[i][0] * e[i - 1];
   }

   return (0);
}
