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

/************************************************************************
    $Id$                        
        Standard Form Digital Filter               
        latice digital filter                  
            double dfs(x, a, m, b, n, buf, bufp)         
                           
            double x             : filter input         
            double a[0]          : gain
            double a[1],...,a[m] : AR coefficints
            int  m               : order of AR part
            double b[0],...,b[n] : MA coefficents
            int  n               : order of MA part
            double buf[]         : buffer
            int *bufp            : pointer for buffer
            return               : filter output   
                1987.11  K.Tokuda               
                1995.12  N.Isshiki               
************************************************************************/

#include <stdio.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

double dfs (double x, double *a, int m, double *b, int n, double *buf, int *bufp)
{
   double y=0.0;
   int i, p;
   int max;

   n++;
   m++;

   (m<n) ? (max=n) : (max=m);

   x = x*a[0];
   for (i=1; i<m; i++) {
      if ((p=*bufp+i)>=max)
         p -= max;
      x -= buf[p] * a[i];
   }
   buf[*bufp] = x;
   for (i=0; i<n; i++) {
      if ((p=*bufp+i)>=max)
         p -= max;
      y += buf[p] * b[i];
   }

   if (--*bufp<0)
      *bufp += max;

   return(y);
}

