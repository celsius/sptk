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
   $Id$               
       NAME:               
                fft - fast fourier transform    
       SYNOPSIS:               
                int   fft(x, y, m);         
                     
                double   x[];   real part      
                double   y[];   imaginary part      
                int      m;     data size      
   
                return : success = 0
                         fault   = -1
       Naohiro Isshiki          Dec.1995    modified   
********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

double *_sintbl=0;
int maxfftsize=0;

static int checkm (const int m)
{
   int k;

   for (k=4; k<=m; k<<=1) {
      if (k==m)
         return(0);
   }
   fprintf(stderr, "fft : m must be a integer of power of 2!\n");

   return(-1);
}

int fft (double *x, double *y, const int m)
{
   int j, lmx, li;
   double *xp, *yp;
   double *sinp,*cosp;
   int lf, lix,tblsize;
   int mv2, mm1;
   double t1, t2;
   double  arg;
   int checkm(const int);

   /**************
   * RADIX-2 FFT *
   **************/

   if (checkm(m))
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
         *sinp++ = sin(arg*(double)j);
      _sintbl[m/2] = 0;
      maxfftsize = m;
   }

   lf = maxfftsize / m;
   lmx = m;

   for(;;) {
      lix = lmx;  
      lmx /= 2;
      if (lmx <= 1) break;
      sinp = _sintbl;
      cosp = _sintbl + maxfftsize/4;
      for (j=0; j<lmx; j++) {
         xp = &x[j];
         yp = &y[j];
         for (li=lix; li<=m ; li+=lix) {
            t1 = *(xp) - *(xp + lmx);
            t2 = *(yp) - *(yp + lmx);
            *(xp) += *(xp + lmx);
            *(yp) += *(yp + lmx);
            *(xp + lmx) = *cosp * t1 + *sinp * t2;
            *(yp + lmx) = *cosp * t2 - *sinp * t1;
            xp += lix;
            yp += lix;
         }
         sinp += lf;
         cosp += lf;
      }
      lf += lf;
   }

   xp = x;
   yp = y;
   for (li=m/2; li--; xp+=2,yp+=2) {
      t1 = *(xp) - *(xp + 1);
      t2 = *(yp) - *(yp + 1);
      *(xp) += *(xp + 1);
      *(yp) += *(yp + 1);
      *(xp + 1) = t1;
      *(yp + 1) = t2;
   }
   
   /***************
   * bit reversal *
   ***************/
   j = 0;
   xp = x;
   yp = y;
   mv2 = m / 2;
   mm1 = m - 1; 
   for (lmx=0; lmx<mm1; lmx++) {
      if ((li=lmx-j)<0) {
         t1 = *(xp);
         t2 = *(yp);
         *(xp) = *(xp + li);
         *(yp) = *(yp + li);
         *(xp + li) = t1;
         *(yp + li) = t2;
      }
      li = mv2;
      while (li<=j) {
         j -= li;
         li /= 2;
      }
      j += li;
      xp = x + j;
      yp = y + j;
   }
 
   return(0);
}
