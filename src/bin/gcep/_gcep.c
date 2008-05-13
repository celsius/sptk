/*
  ---------------------------------------------------------------  
            Speech Signal Processing Toolkit (SPTK)

                      SPTK Working Group                           
                                                                   
                  Department of Computer Science                   
                  Nagoya Institute of Technology                   
                               and                                 
   Interdisciplinary Graduate School of Science and Engineering    
                  Tokyo Institute of Technology                    
                                                                   
                     Copyright (c) 1984-2007                       
                       All Rights Reserved.                        
                                                                   
  Permission is hereby granted, free of charge, to use and         
  distribute this software and its documentation without           
  restriction, including without limitation the rights to use,     
  copy, modify, merge, publish, distribute, sublicense, and/or     
  sell copies of this work, and to permit persons to whom this     
  work is furnished to do so, subject to the following conditions: 
                                                                   
    1. The source code must retain the above copyright notice,     
       this list of conditions and the following disclaimer.       
                                                                   
    2. Any modifications to the source code must be clearly        
       marked as such.                                             
                                                                   
    3. Redistributions in binary form must reproduce the above     
       copyright notice, this list of conditions and the           
       following disclaimer in the documentation and/or other      
       materials provided with the distribution.  Otherwise, one   
       must contact the SPTK working group.                        
                                                                   
  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF TECHNOLOGY,   
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM   
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF         
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE   
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          
  PERFORMANCE OF THIS SOFTWARE.                                    
                                                                   
  ---------------------------------------------------------------  
*/

/****************************************************************

    $Id: _gcep.c,v 1.12 2008/05/13 06:14:25 heigazen Exp $

    Generalized Cepstral Analysis

        int gcep(xw, flng, gc, m, g, itr1, itr2, d, e, f, itype);

        double    *xw  : input sequence
        int       flng : frame length
        double    *gc  : generalized cepstrum
        int       m    : order of generalized cepstrum
        double    g    : gamma
        int       itr1 : minimum number of iteration
        int       itr2 : maximum number of iteration
        double    d    : end condition
        double    e    : initial value for log-periodgram
        double    f    : mimimum value of the determinant 
                         of the normal matrix
        int      itype : input data type

        return value :    0 -> completed by end condition
                          -1-> completed by maximum iteration

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

int gcep (double *xw, const int flng, double *gc, const int m, const double g, const int itr1, const int itr2, 
          const double d, const double e, const double f, const int itype)
{
   int i, j, flag=0;
   double t, s, dd=0.0;
   static double *x=NULL, *y, *cr, *ci, *rr, *hr, *hi, *er, *ei;
   static int size;

   if (x==NULL) {
      x = dgetmem(9*flng);
      size = flng;

      y =  x  + flng;   cr = y  + flng;
      ci = cr + flng;   rr = ci + flng;
      hr = rr + flng;   hi = hr + flng;
      er = hi + flng;   ei = er + flng;
   }
   
   if (flng>size) {
      free(x);
      x = dgetmem(9*flng);
      size = flng;

      y =  x  + flng;   cr = y  + flng;
      ci = cr + flng;   rr = ci + flng;
      hr = rr + flng;   hi = hr + flng;
      er = hi + flng;   ei = er + flng;
   }

   movem(xw, x, sizeof(*x), flng);
    
   switch (itype) {
   case 0:   /* windowed data sequence */
      fftr(x, y, flng);
      for (i=0; i<flng; i++) {
         x[i] = x[i]*x[i] + y[i]*y[i] + e;  /*  periodegram  */
      }
      break;
   case 1:   /* dB */
      for (i=0; i<=flng/2; i++) {
         x[i] /= 20.0 / log(10.0);  /* dB -> amplitude spectrum */
         x[i] = x[i]*x[i]+e;  /* amplitude -> periodgram */
      }
      break;
   case 2:  /* log */
      for (i=0; i<=flng/2; i++) {
         x[i] = exp(x[i]);  /* log -> amplitude spectrum */
         x[i] = x[i]*x[i]+e;  /* amplitude -> periodgram */
      }
      break;
   case 3:  /* amplitude */
      for (i=0; i<=flng/2; i++) {
         x[i] = x[i]*x[i]+e;  /* amplitude -> periodgram */
      }
      break;
   case 4:  /* periodgram */
      for (i=0; i<=flng/2; i++) {
         x[i] = x[i]+e;
      }
      break;
   default:
     fprintf(stderr, "mgcep : Input type %d is not supported!\n", itype);
     exit(1);
   }
   if (itype>0) {
      for (i=1; i<flng/2; i++)
         x[flng-i] = x[i];
   }
   for (i=0; i<flng; i++)
      cr[i] = log(x[i]);

   /*  initial value of generalized cepstrum  */
   ifftr(cr, y, flng);   /*  x : IFFT[x]  */
   cr[0] = exp(cr[0] / 2);
   gc2gc(cr, m, 0.0, gc, m, g);  /*  gc : generalized cepstrum  */

   /*  Newton-Raphson method  */
   for (j=1; j<=itr2; j++) {
      fillz(cr, sizeof(*cr), flng);
      movem(&gc[1], &cr[1], sizeof(*cr), m);
      fftr(cr, ci, flng);         /*  cr+jci : FFT[gc]  */
   
      for (i=0; i<flng; i++) {
         t = x[i] / agexp(g, cr[i], ci[i]);
         cr[i] = 1 + g * cr[i];
         ci[i] = g * ci[i];
         s = cr[i] * cr[i] + ci[i] * ci[i];
         rr[i] = t / s;
         hr[i] = (cr[i] * cr[i] - ci[i] * ci[i]) * t / (s * s);
         hi[i] = 2 * cr[i] * ci[i] * t / (s * s);
         er[i] = cr[i] * t / s;
         ei[i] = ci[i] * t / s;
      }
   
      ifftr(rr, y, flng);         /*  rr : r(k)  */
      ifft(hr, hi, flng);         /*  hr : h(k)  */
      ifft(er, ei, flng);         /*  er : e(k)  */
      s = gc[0];            /*  gc[0] : gain  */
      
      for (i=1,t=0.0; i<=m; i++)
         t += er[i] * gc[i];
      
      t = er[0] + g * t;
      t = sqrt(fabs(t));

      if (j>=itr1) {
         if (fabs((t-dd)/t)<d) {
            flag = 1;
            break;
         }
         dd = t;
      }

      for (i=2; i<=m+m; i++) hr[i] *= 1 + g;

      if (theq(rr, &hr[2], &y[1], &er[1], m, f)) {
         fprintf(stderr, "gcep : Error in theq() at %dth iteration!\n", j);
         exit(1);
      }

      gc[0] = t;
      
      for (i=1; i<=m; i++) gc[i] += y[i];
   }

   if (flag) return(0);
   else return(-1);
}

