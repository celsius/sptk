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

/******************************************************
 $Id: _df2.c,v 1.8 2007/10/08 16:49:31 heigazen Exp $
    2nd order standard form digital filter
        double df2(x,sf,f0p,wbp,f0z,wbz,fp,fz,buf,bufp)

        double   x     : filter input
        double   sf    : sampling frequency
        double   f0p   : center frequency of pole
        double   wbp   : band width of pole
        double   f0z   : center frequency of zero
        double   wbz   : band witdth of zero
        int      fp    : if pole is exist then fp = 1 
        int      fz    : if zero is exist then fz = 1
        double   buf[] : buffer ( which require 3 )
        int      *bufp : work pointer
   
        return         : filter output

                         Dec.1995 Naohiro Isshiki
*******************************************************/    

#include <stdio.h>
#include <math.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

double df2 (const double x, const double sf, const double f0p, const double wbp, const double f0z, const double wbz, 
            const int fp, const int fz, double *buf, int *bufp)
{
   double a[3], b[3];
   double p, e;
 
   p = 4 * atan(1.0) / sf;
   e = exp(-p * wbz);

   a[0] = 1.0;
   if (fz) {
      a[1] = -2 * e * cos(2 * p * f0z);
      a[2] = e * e;
   }
   else {
      a[1] = 0;
      a[2] = 0;
   }
  
   e = exp(-p * wbp);
   b[0] = 1.0;
   if (fp) {
      b[1] = -2 * e * cos(2 * p * f0p);
      b[2] = e * e;
   } 
   else {
      b[1] = 0;
      b[2] = 0;
   }
 
   return(dfs(x,b,2,a,2,buf,bufp));
}


