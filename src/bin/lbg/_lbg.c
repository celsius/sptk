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

/************************************************************************

    $Id: _lbg.c,v 1.8 2007/09/10 12:49:23 heigazen Exp $

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
#include <SPTK.h>

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

