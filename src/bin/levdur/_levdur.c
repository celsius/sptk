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

    $Id: _levdur.c,v 1.9 2007/09/12 08:37:23 heigazen Exp $

    Solve an Autocorrelation Normal Equation
    Using Levinson-Durbin Method

       int levdur(r, a, m, eps);

       double  *r    : autocorrelation sequence
       double  *a    : LP coefficients
       int     m     : order of LPC
       double  eps   : singular check (eps(if -1., 1.0e-6 is assumed))

       return  value :  0  -> normally completed
                        -1 -> abnormally completed
                        -2 -> unstable LPC

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>

int levdur (double *r, double *a, const int m, double eps)
{
   int l, k, flag=0;
   double rmd, mue;
   static double *c=NULL;
   static int size;

   if (c==NULL) {
      c = dgetmem(m+1);
      size = m;
   }

   if (m>size) {
      free(c);
      c = dgetmem(m+1);
      size = m;
   }

   if (eps<0.0) eps = 1.0e-6;
   if ((rmd=r[0]) <= eps) return(-1);   
   a[0] = 0.0;

   for (l=1; l<=m; l++) {
      mue = -r[l];
      for (k=1; k<l; k++)
         mue -= c[k] * r[l - k];
      mue = mue / rmd;

      for (k=1; k<l; k++)
         a[k] = c[k] + mue * c[l - k];
      a[l] = mue;

      rmd = (1.0 - mue * mue) * rmd;
      if (((rmd<0.0) ? -rmd : rmd) <= eps) return(-1);
      if (((mue<0.0) ? -mue : mue) >= 1.0) flag = -2;

      for (k=0; k<=l; k++) c[k] = a[k];
   }
   a[0] = sqrt(rmd);
   
   return(flag);
}

