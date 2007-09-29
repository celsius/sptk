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

    Extract mel-generalized cepstral coefficients for given spectrum

        int sp2mgc(xw, flng, b, m, a, g, itr1, itr2, dd, e);

         double   *xw   : input sequence
         int      flng  : frame length
         double   *b    : coefficient b'(m)
         int      m     : order of mel cepstrum
         double   a     : alpha
         double   g     : gamma
         int      n     : order of recursions
         int      itr1  : minimum number of iteration
         int      itr2  : maximum number of iteration
         double   dd    : end condition
         double   e     : initial value for log-periodgram

         return   value :  0 -> completed by end condition
                          -1-> completed by maximum iteration

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>

int sp2mgc (double *xw, const int flng, double *b, const int m, const double a, const double g, const int n, 
            const int itr1, const int itr2, const double dd, const double e, const int itype)
{
   int i, j, flag=0;
   static double *x=NULL, *d;
   static int size_x, size_c;
   double ep, epo;

   if (x==NULL) {
      x = dgetmem(flng);
      size_x = flng;

      d = dgetmem(m+1);
      size_c = m;
   }
   
   if (flng>size_x) {
      free(x);
      x = dgetmem(flng);
      size_x = flng;
   }
   if (m>size_c) {
      free(d);
      d = dgetmem(m+1);
      size_c = m;
   }

   movem(xw, x, sizeof(*x), flng);

   /* Amplitude Spectrum*/ 
   switch (itype) {
   case 0:   /* dB */
      for (i=0; i<=flng/2; i++) {
         x[i] /= 20.0 / log(10.0);  /* dB -> amplitude spectrum */
         x[i] = x[i]*x[i]+e;  /* amplitude -> periodgram */
      }
      break;
   case 1:  /* log */
      for (i=0; i<=flng/2; i++) {
         x[i] = exp(x[i]);  /* log -> amplitude spectrum */
         x[i] = x[i]*x[i]+e;  /* amplitude -> periodgram */
      }
      break;
   case 2:  /* amplitude */
      for (i=0; i<=flng/2; i++) {
         x[i] = x[i]*x[i]+e;  /* amplitude -> periodgram */
      }
      break;
   default:  /* periodgram */
      for (i=0; i<=flng/2; i++) {
         x[i] = x[i]+e;
      }
   }
   for (i=1; i<flng/2; i++)
      x[flng-i] = x[i];


   /* initial value */
   fillz(b, sizeof(*b), m+1);
   ep = newton(x, flng, b, m, a, -1.0, n, 0);

   if (g!=-1.0) {
      if (a!=0.0) {
         ignorm(b, b, m, -1.0); /*  K, b'r(m)    -> br(m)         */
         b2mc(b, b, m, a);      /*  br(m)        -> c~r(m)        */
         gnorm(b, d, m, -1.0);  /*  c~r(m)       -> K~, c~'r(m)   */
      }
      else
         movem(b, d, sizeof(*b), m+1);

      gc2gc(d, m, -1.0, b, m, g); /*  K~, c~'r(m)  -> K~, c~'r'(m)  */

      if (a!=0.0) {
         ignorm(b, b, m, g);  /*  K~, c~'r'(m) -> c~r(m)        */
         mc2b(b, b, m, a);    /*  c~r(m)       -> br(m)         */
         gnorm(b, b, m, g);   /*  br(m)        -> K, b'r'(m)    */
      }
   }

   /*  Newton-Raphson method  */
   if (g!=-1.0) {
      for (j=1; j<=itr2; j++) {
         epo = ep;
         ep = newton(x, flng, b, m, a, g, n, j);

         if (j >= itr1)
            if (fabs((epo - ep)/ep)<dd) {
               flag = 1;
               break;
            }
      }
   }

   if (flag) return(0);
   else     return(-1);
}
