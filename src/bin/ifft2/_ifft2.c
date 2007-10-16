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
*  $Id: _ifft2.c,v 1.10 2007/10/16 02:21:17 heigazen Exp $       *
*  ifft2 : two dimensional inverse fast Fourier                 *
*      transform                                                *
*                                                               *
*      int ifft2( x, y, n )                                     *
*                                                               *
*      real   x[0]...x[n*n-1]; real part                        *
*      real   y[0]...y[n*n-1]; imaginary part                   *
*      int    n;               size of IDFT                     *
*                                                               *
*                     T. Kobayashi   May, 1989.                 *
****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

int ifft2 (double x[], double y[], const int n)
{
   double *xq, *yq;
   static double *xb=NULL, *yb;
   double *xp, *yp;
   int i, j;
   static int size_f;
   
   if (xb==NULL) {
      size_f = 2*n;
      xb = dgetmem(size_f);
      yb = xb + n;
   }
   if (2*n>size_f) {
      free(xb);
      size_f = 2 * n;
      xb = dgetmem(size_f);
      yb = xb + n;
   }      
   
   for (i=0; i<n; i++) {
      xp = xb;   xq = x + i;
      yp = yb;   yq = y + i;
      
      for (j=n; --j>=0; xq+=n,yq+=n) {
         *xp++ = *xq;
         *yp++ = *yq;
      }
      
      if (ifft( xb, yb, n)<0)
         return(-1);

      xp = xb;   xq = x + i;
      yp = yb;   yq = y + i;
    
      for (j=n; --j>=0; xq+=n,yq+=n) {
         *xq = *xp++;
         *yq = *yp++;
      }
   }

   for (i=n,xp=x,yp=y; --i>=0; xp+=n,yp+=n) {
      if (ifft(xp, yp, n )<0)
         return(-1);
   }

   return(0);
}

