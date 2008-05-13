/*
  ---------------------------------------------------------------  
            Speech Signal Processing Toolkit (SPTK)

                      SPTK Working Group                           
                                                                   
                  Department of Computer Science                   
                  Nagoya Institute of Technology                   
                               and                                 
   Interdisciplinary Graduate School of Science and Engineering    
                  Tokyo Institute of Technology                    
                                                                   
                     Copyright (c) 1984-2008                       
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

/**********************************************************************

    $Id$

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

double pitch (double *xw, const int l, const double thresh, const int low, const int high, 
              const double eps, const int m, const int itr1, const int itr2, const double end)
{
   static double *x=NULL,*y, *c;
   double voiced,max,p=0.0;
   int i;

   if (x==NULL) {
      x = dgetmem(3*l);
      y = x + l;
      c = y + l;
   }

   movem(xw,x,sizeof(*x),l);

   /* voiced/unvoiced detection */
   uels(x, l, c, m, itr1, itr2, end, eps, 0);
   fillz(c+m,l-m,sizeof(double));
   fftr(c,y,l);

   voiced = 0.0;
   for (i=4*l/256;i<=17*l/256;i++)
      voiced += c[i];
   voiced /= 14 * l / 256;

   fftr(x, y, l);
   for (i=0; i<l; i++)
      x[i] = log(x[i]*x[i] + y[i]*y[i] + eps);

   if (voiced>thresh) {
      fftr(x, y, l);
      for (i=0; i<l; i++) {
         x[i] /= l;
         x[i] *= i;
      }
      max = 0.0;
      for (i=low; i<high; i++)
         if (max<x[i]) {
            p = (float)i;
            max = x[i];
         }
   }
   else
      p = 0.0;
      
   return(p);
}

