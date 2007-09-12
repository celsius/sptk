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

    $Id: _zerodf.c,v 1.7 2007/09/12 08:37:18 heigazen Exp $

    All Zero Digital Filter

       double zerodf(x, b, m, d)

       double   x     : input 
       double  *b     : MA coefficients 
       int      m     : order of coefficients
       double  *d     : delay

       return   value : filtered data

*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>

double zerodf (double x, double *b, int m, double *d)
{
   double out;

   out = b[0]*x;

   for (m--; m>0; m--) {
      out += b[m+1] * d[m];
      d[m] = d[m-1];
   }
   out += b[1] * d[0];
   d[0] = x;

   return(out);
}

double zerodft (double x, double *b, const int m, double *d)
{
   int i;
   double out;

   out = b[0]*x + d[0];

   for (i=1; i<m; i++)
      d[i-1] = b[i] * x + d[i];

   d[m-1] = b[m] * x;

   return(out);
}

double zerodf1 (double x, double *b, int m, double *d)
{
   double out;

   out = x;
   for (m--; m>0; m--) {
      out += b[m+1] * d[m];
      d[m] = d[m-1];
   }
   out += b[1] * d[0];
   d[0] = x;

   return(out);
}

double zerodf1t (double x, double *b, const int m, double *d)
{
   int i;
   double out;

   out = x + d[0];

   for (i=1; i<m; i++)
      d[i-1] = b[i] * x + d[i];

   d[m-1] = b[m] * x;

   return(out);
}

