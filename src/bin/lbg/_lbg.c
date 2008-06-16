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
/*                1996-2008  Nagoya Institute of Technology          */
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

/************************************************************************

    $Id: _lbg.c,v 1.12 2008/06/16 05:48:37 heigazen Exp $

    LBG Algorithm for Vector Qauntizer Design

       void lbg(x, l, tnum, icb, icbsize, cb, ecbsize, delta, end)

       double *x      :   training vector
       double l       :   length of vector
       int    tnum    :   number of training vector
       double *icb    :   initial codebook
       int    icbsize :   initial codebook size
       double *cb     :   final codebook
       int    ecbsize :   final codebook size
       double delta   :   splitting factor
       double end     :   end condition

************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

#define MAXVALUE 1e23
#define abs(x) ( (x<0) ? (-(x)) : (x) )


void lbg (double *x, const int l, const int tnum, double *icb, int icbsize, double *cb, const int ecbsize, const double delta, const double end)
{
   int i, j, k, maxindex;
   static int *cntcb, *tindex, size, sizex, sizecb;
   double d0, d1, dl, err;
   static double *rnd=NULL, *cb1;
   double *p, *q, *r;

   if (rnd==NULL) {
      rnd = dgetmem(l);
      cb1 = dgetmem(ecbsize*l);
      tindex = (int *)dgetmem(tnum);
      cntcb  = (int *)dgetmem(ecbsize);
      size = l;
      sizex = tnum;
      sizecb = ecbsize;
   }
   if (l>size) {
      free(rnd);
      free(cb1);
      rnd = dgetmem(l);
      cb1 = dgetmem(ecbsize*l);
      size = l;
   }
   if (tnum>sizex) {
      free(tindex);
      tindex = (int *)dgetmem(tnum);
      sizex = tnum;
   }
   if (sizecb>ecbsize) {
      free(cb1);
      free(cntcb);
      cb1 = dgetmem(ecbsize*l);
      cntcb = (int *)dgetmem(ecbsize);
   }

   movem(icb, cb, sizeof(*icb), icbsize*l);

   for ( ; icbsize*2 <= ecbsize; ) {
      q = cb;
      r = cb + icbsize*l;   /* splitting */
      for (i=0; i<icbsize; i++) {
         nrand(rnd, l, i);
         for (j=0; j<l; j++) {
            dl = delta * rnd[j];
            *r = *q - dl;
            r++;
            *q = *q + dl;
            q++;
         }
      }
      icbsize *= 2;

      d0 = MAXVALUE;
      for ( ;; ) {
         fillz((double *)cntcb, sizeof(*cntcb), icbsize);
         d1 = 0.0;
         p = x;
         for (i=0; i<tnum; i++,p+=l) {
            tindex[i] = vq(p, cb, l, icbsize);
            cntcb[tindex[i]]++;

            q = cb + tindex[i] * l;
            d1 += edist(p, q, l);
         }


         d1 /= tnum;
         err = abs((d0 - d1) / d1);

         if (err<end)  break; /* check distortion */


         d0 = d1;
         fillz(cb1, sizeof(*cb), icbsize*l);

         p = x;    /* get new centroid */
         for (i=0; i<tnum; i++) {
            q = cb1 + tindex[i] * l;
            for (j=0; j<l; j++) *q++ += *p++;
         }

         k = maxindex = 0;
         for (i=0; i<icbsize; i++)
            if (cntcb[i]>k) {
               k = cntcb[i];
               maxindex = i;
            }


         q = cb;
         r = cb1;
         for (i=0; i<icbsize; i++,r+=l,q+=l)
            if (cntcb[i]>0)
               for (j=0; j<l; j++)
                  q[j] = r[j] / (double) cntcb[i];
            else {
               nrand(rnd, l, i);
               p = cb + maxindex * l;
               for (j=0; j<l; j++)
                  q[j] = p[j] + delta * rnd[j];
            }
      }
      if (icbsize==ecbsize) break;
   }
   
   return;
}

