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

    $Id$

    Mel-Cepstral Analysis

        int mcep(xw, flng, mc, m, a, itr1, itr2, dd, e);

        double   *xw   : input sequence
        int      flng  : frame length
        double   *mc   : mel cepstrum
        int      m     : order of mel cepstrum
        double   a     : alpha
        int      itr1  : minimum number of iteration
        int      itr2  : maximum number of iteration
        double   dd    : end condition
        double   e     : initial value for log-periodgram
        double   f     : mimimum value of the determinant 
                         of the normal matrix
                         
        return   value :    0 -> completed by end condition
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

int mcep (double *xw, const int flng, double *mc, const int m, const double a, const int itr1, const int itr2, 
          const double dd, const double e, const double f)
{
   int i, j;
   int flag=0, f2, m2;
   double t, s;
   static double *x=NULL, *y, *c, *d, *al, *b;
   static int size_x, size_d;

   if (x==NULL) {
      x = dgetmem(3*flng);
      y = x + flng;
      c = y + flng;
      size_x = flng;

      d = dgetmem(3*m+3);
      al = d  + (m+1);
      b  = al + (m+1);
      size_d = m;
   }
   if (flng>size_x) {
      free(x);
      x = dgetmem(3*flng);
      y = x + flng;
      c = y + flng;
      size_x = flng;
   }
   if (m>size_d) {
      free(d);
      d = dgetmem(3*m+3);
      al = d  + (m+1);
      b  = al + (m+1);
      size_d = m;
   }

   f2 = flng / 2;
   m2 = m + m;

   movem(xw, x, sizeof(*x), flng);

   /*  power spectrum  */
   fftr(x, y, flng);
   for (i=0; i<flng; i++) {
      x[i] = x[i]*x[i] + y[i]*y[i] + e;
      if (x[i] <= 0) {
         fprintf(stderr, "mcep : The log periodogram has '0', use '-e' option!\n");
         exit(1);
      }
      c[i] = log(x[i]);
   }

   /*  1, (-a), (-a)^2, ..., (-a)^M  */
   al[0] = 1.0;
   for (i=1; i<=m; i++)
      al[i] = -a * al[i-1];

   /*  initial value of cepstrum  */
   ifftr(c, y, flng);            /*  c : IFFT[x]  */

   c[0] /= 2.0;
   c[flng/2] /= 2.0;
   freqt(c, f2, mc, m, a);         /*  mc : mel cep.  */
   s = c[0];

   /*  Newton Raphson method  */
   for (j=1; j<=itr2; j++) {
      fillz(c, sizeof(*c), flng);
      freqt(mc, m, c, f2, -a);      /*  mc : mel cep.  */
      fftr(c, y, flng);         /*  c, y : FFT[mc]  */
      for (i=0; i<flng; i++)
         c[i] = x[i] / exp(c[i] + c[i]);
      ifftr(c, y, flng);
      frqtr(c, f2, c, m2, a);         /*  c : r(k)  */

      t = c[0];
      if (j>=itr1) {
         if (fabs((t - s)/t)<dd) {
            flag = 1;
            break;
         }
         s = t;
      }

      for (i=0; i<=m; i++)
         b[i] = c[i] - al[i];
      for (i=0; i<=m2; i++)  y[i] = c[i];
      for (i=0; i<=m2; i+=2) y[i] -= c[0];
      for (i=2; i<=m;  i+=2) c[i] += c[0];
      c[0] += c[0];

      if (theq(c, y, d, b, m+1, f)) {
         fprintf(stderr,"mcep : Error in theq() at %dth iteration !\n", j);
         exit(1);
      }

      for (i=0; i<=m; i++) mc[i] += d[i];
   }

   if (flag) return(0);
   else return(-1);

}

/***************************************************************

    Frequency Transformation for Calculating Coefficients

        void frqtr(c1, m1, c2, m2, a)

        double *c1   : minimum phase sequence
        int m1       : order of minimum phase sequence
        double *c2   : warped sequence
        int m2       : order of warped sequence
        double a     : all-pass constant

***************************************************************/

void frqtr (double *c1, int m1, double *c2, int m2, const double a)
{
   int i, j;
   static double *d=NULL, *g;
   static int size;

   if (d==NULL) {
      size = m2;
      d = dgetmem(size+size+2);
      g = d + size + 1;
   }

   if (m2>size) {
      free(d);
      size = m2;
      d = dgetmem(size+size+2);
      g = d + size + 1;
   }

   fillz(g, sizeof(*g), m2+1);

   for (i=-m1; i<=0; i++) {
      if (0 <= m2) {
         d[0] = g[0];
         g[0] = c1[-i];
      }
      for (j=1; j<=m2; j++)
         g[j] = d[j-1] + a*((d[j] = g[j]) - g[j-1]);
   }

   movem(g, c2, sizeof(*g), m2+1);

   return;
}
