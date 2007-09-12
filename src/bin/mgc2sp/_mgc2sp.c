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

    $Id: _mgc2sp.c,v 1.7 2007/09/12 08:37:25 heigazen Exp $

    Transformation mel-generalized cepstrum to log spectrum

        void mgc2sp(mgc, m, a, g, sp, flng)

        double *mgc  : mel-generalize cepstrum
        int     m    : order of mel-generalized cepstrum
        double  a    : alpha
        double  g    : gamma
        double *x    : real part of spectrum
        double *y    : imaginary part of spectrum
        int     flng : FFT length

***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>

void mgc2sp (double *mgc, const int m, const double a, const double g, double *x, double *y, const int flng)
{
   static double *c=NULL;
   static int size;

   if (c==NULL) {
      c = dgetmem(flng/2 + 1);
      size = flng;
   }
   if (flng>size) {
      free(c);
      c = dgetmem(flng/2 + 1);
      size = flng;
   }

   mgc2mgc(mgc, m, a, g, c, flng/2, 0.0, 0.0);
   c2sp(c, flng/2, x, y, flng);
   
   return;
}
