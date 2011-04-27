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
/*                1996-2011  Nagoya Institute of Technology          */
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

/**********************************************************************

    $Id: _pitch.c,v 1.19 2011/04/27 13:46:43 mataki Exp $

    Pitch Extract

        double pitch(xw, l, thresh, low, high, eps, m, itr1, itr2, end)

        double *xw     : windowed data sequence
        int     l      : frame length (fft size)
        double  thresh : voiced/unvoiced threshold
        int     low    : minmum points to search for
        int     high   : maximum points to search for
        double  eps    : small value for log
        int     m      : order of cepstrum
        int     itr1   : minimum number of iteration
        int     itr2   : maximum number of iteration
        double  end    : end condition

************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

double pitch(double *xw, const int l, const double thresh, const int low,
             const int high, const double eps, const int m, const int itr1,
             const int itr2, const double end)
{
   static double *x = NULL, *y, *c;
   double voiced, max, p = 0.0;
   int i;

   if (x == NULL) {
      x = dgetmem(3 * l);
      y = x + l;
      c = y + l;
   }

   movem(xw, x, sizeof(*x), l);

   /* voiced/unvoiced detection */
   uels(x, l, c, m, itr1, itr2, end, eps, 0);
   fillz(c + m, l - m, sizeof(double));
   fftr(c, y, l);

   voiced = 0.0;
   for (i = 4 * l / 256; i <= 17 * l / 256; i++)
      voiced += c[i];
   voiced /= 14 * l / 256;

   fftr(x, y, l);
   for (i = 0; i < l; i++)
      x[i] = log(x[i] * x[i] + y[i] * y[i] + eps);

   if (voiced > thresh) {
      fftr(x, y, l);
      for (i = 0; i < l; i++) {
         x[i] /= l;
         x[i] *= i;
      }
      max = 0.0;
      for (i = low; i < high; i++)
         if (max < x[i]) {
            p = (float) i;
            max = x[i];
         }
   } else
      p = 0.0;

   return (p);
}
