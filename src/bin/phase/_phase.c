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

/*******************************************************************
  $Id$
  comupte phase of digital filter or real seaquence
     phase(p, mp, z, mz, ph, flng)

     double *p    : numerator cofficients
     int     mp   : order of numerator polynomial
     double *z    : denominator cofficients
     int     mz   : order of denominator polynomilal
     doble *ph    : phase
     int    flng  : FFT size
     int    unlap : unlapping

                                       Naohiro Isshiki  Feb.1995
********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SPTK.h>

void phase (double *p, const int mp, double *z, const int mz, double *ph, const int flng, const int unlap)
{
   static double *x;
   static int fsize=0;
   double  *y, *xx, *yy,*py;
   int no, i, offset;
   double pi;

   pi = atan(1.) * 4.;

   no = flng / 2 + 1;

   if (flng>fsize) {
      if (x!=NULL)
         free(x);
      fsize = flng;
      x = dgetmem(4 * flng + no);
   }
   y = &x[flng];
   xx = &y[flng];
   yy = &xx[flng];
   py = &yy[flng];

   fillz(x, sizeof(*x), flng);
   fillz(xx, sizeof(*xx), flng);
   movem(z, x, mz+1, sizeof(*z));
   movem(p, xx, mp+1, sizeof(*p));

   fftr(x, y, flng);
   xx[0] = 1;
   fftr(xx, yy, flng);
   for (i = 0; i<no; i++) {
      ph[i] = x[i] * xx[i] + y[i] * yy[i];
      py[i] = y[i] * xx[i] - x[i] * yy[i];
   }
   offset = 0;
   i = 0;
   ph[i] = atan2(py[i], ph[i]) / pi;
   i++;
   for (; i<no; i++) {
      ph[i] = atan2(py[i], ph[i]) / pi;
      if (unlap) {
         if (ph[i - 1] - ph[i] - offset>1)
            offset += 2;
         else if (ph[i] + offset - ph[i - 1]>1)
            offset -= 2;
         ph[i] += offset;
      }
   }
   
   return;
}

