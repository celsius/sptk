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

/********************************************************
 $Id: _fftr.c,v 1.10 2007/10/16 02:20:54 heigazen Exp $                     
 
 NAME:                     
        fftr - Fast Fourier Transform for Double sequence      
 SYNOPSIS:                                             
        int   fftr(x, y, m)            
                     
        double  x[];   real part of data      
        double  y[];   working area         
        int     m;     number of data(radix 2)      
                Naohiro Isshiki    Dec.1995   modified
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

extern double *_sintbl;
extern int maxfftsize;

int fftr (double *x, double *y, const int m)
{
   int i, j;
   double *xp, *yp, *xq;
   double *yq;
   int mv2, n, tblsize;
   double xt, yt, *sinp, *cosp;
   double arg;
   
   mv2 = m / 2;

   /* separate even and odd  */
   xq = xp = x;
   yp = y;
   for (i=mv2; --i>=0; ) {
      *xp++ = *xq++;
      *yp++ = *xq++;
   }

   if (fft(x, y, mv2)==-1)        /* m / 2 point fft */
      return(-1);


   /***********************
   * SIN table generation *
   ***********************/

   if ((_sintbl==0) || (maxfftsize<m)) {
      tblsize=m-m/4+1;
      arg=PI/m*2;
      if (_sintbl!=0)
         free(_sintbl);
      _sintbl = sinp = dgetmem(tblsize);
      *sinp++ = 0;
      for (j=1; j<tblsize; j++)
         *sinp++ = sin( arg * (double)j);
      _sintbl[m/2] = 0;
      maxfftsize = m;
   }

   n = maxfftsize / m;
   sinp = _sintbl;
   cosp = _sintbl + maxfftsize/4;

   xp = x;
   yp = y;
   xq = xp + m;
   yq = yp + m;
   *(xp + mv2) = *xp - *yp;
   *xp = *xp + *yp;
   *(yp + mv2) = *yp = 0;
   
   for (i=mv2,j=mv2-2; --i ; j-=2) {
      ++xp;
      ++yp;
      sinp += n;
      cosp += n;
      yt = *yp + *(yp + j);
      xt = *xp - *(xp + j);
      *(--xq) = (*xp + *(xp + j) + *cosp * yt - *sinp * xt) * 0.5;
      *(--yq) = (*(yp + j) - *yp + *sinp * yt + *cosp * xt) * 0.5;
   }

   xp = x + 1;
   yp = y + 1;
   xq = x + m;
   yq = y + m;
   
   for (i=mv2; --i; ) {
      *xp++ =   *(--xq); 
      *yp++ = -(*(--yq));
   }
   
   return(0);
}

