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

/***************************************************************

    $Id: _lpc2par.c,v 1.9 2007/10/16 02:21:13 heigazen Exp $

    Transformation LPC to PARCOR

        int lpc2par(a, k, m)

        double  *a : LP coefficients
        double  *k : PARCOR coefficients
        int      m : order of LPC

        return value :  0  -> normally completed
                        -1 -> abnormally completed

****************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

int lpc2par (double *a, double *k, const int m)
{
   int i, n, flg=0;
   double s;
   static double *kk=NULL, *aa;
   static int size;

   if (kk==NULL) {
      kk = dgetmem(m+m+2);
      aa = kk + m + 1;
      size = m;
   }

   if (m>size) {
      free(kk);
      kk = dgetmem(m+m+2);
      aa = kk + m + 1;
      size = m;
   }

   movem(a, aa, sizeof(*aa), m+1);

   kk[0] = aa[0];
   for (n=m; n>=1; n--) {
      movem(&aa[1], &kk[1], sizeof(*aa), n);

      if (kk[n]>=1.0 || kk[n]<=-1.0) flg = -1;

      s = 1.0 - kk[n] * kk[n];
      for (i=1; i<n; i++)
         aa[i] = (kk[i] - kk[n] * kk[n-i]) / s;
   }
   movem(kk, k, sizeof(*kk), m+1);
   
   return(flg);
}

