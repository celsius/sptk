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

/***********************************************************************

    $Id$

    Frequency and Generalized Cepstral Transformation

        void mgc2mgc(c1, m1, a1, g1, c2, m2, a2, g2)

        double *c1   : minimum phase sequence (input)
        int     m1   : order of c1
        double  a1   : alpha of c1
        double  g1   : gamma of c1
        double *c2   : transformed sequence (output)
        int     m2   : order of c2
        double  a2   : alpha of c2
        double  g2   : gamma of c2

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

void mgc2mgc (double *c1, const int m1, const double a1, const double g1, double *c2, const int m2, const double a2, const double g2)
{
   double a;
   static double *ca=NULL;
   static int size_a;

   if (ca==NULL) {
      ca = dgetmem(m1+1);
      size_a = m1;
   }
   if (m1>size_a) {
      free(ca);
      ca = dgetmem(m1+1);
      size_a = m1;
   }

   a = (a2 - a1) / (1 - a1*a2);

   if (a==0) {
      movem(c1, ca, sizeof(*c1), m1+1);
      gnorm(ca, ca, m1, g1);
      gc2gc(ca, m1, g1, c2, m2, g2);
      ignorm(c2, c2, m2, g2);
   }
   else {
      freqt(c1, m1, c2, m2, a);
      gnorm(c2, c2, m2, g1);
      gc2gc(c2, m2, g1, c2, m2, g2);
      ignorm(c2, c2, m2, g2);
   }
   
   return;
}

