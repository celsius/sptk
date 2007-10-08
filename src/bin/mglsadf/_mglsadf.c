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

    $Id: _mglsadf.c,v 1.9 2007/10/08 16:49:35 heigazen Exp $

    MGLSA Digital Filter

        double mglsadf(x, b, m, a, n, d)

        double  x     : input
        double  *b    : filter coefficients (K, g*b'(1), ..., g*b'(m))
        int     m     : order of cepstrum
        double  a     : alpha
        int     n     : -1/gamma
        double  *d    : delay

        return  value : filtered data

*****************************************************************/

#include <stdio.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

static double mglsadff (double x, double *b, const int m, const double a, double *d)
{
   int i;
   double y, aa;

   aa = 1 - a * a;

   y = d[0] * b[1];
   for (i=1; i<m; i++) {
      d[i] += a * (d[i+1] - d[i-1]);
      y += d[i] * b[i+1];
   }
   x -= y;

   for (i=m; i>0; i--)
      d[i] = d[i-1];
   d[0] = a * d[0] + aa * x;

   return (x);
}

double mglsadf (double x, double *b, const int m, const double a, const int n, double *d)
{
   int i;
   
   for (i=0; i<n; i++)
      x = mglsadff(x, b, m, a, &d[i*(m+1)]);

   return(x);
}

static double mglsadff1 (double x, double *b, const int m, const double a, const double g, double *d)
{
   int i;
   double y, aa;

   aa = 1 - a * a;

   y = d[0] * b[1];
   for (i=1; i<m; i++) {
      d[i] += a * (d[i+1] - d[i-1]);
      y += d[i] * b[i+1];
  }
  x -= g * y;

  for (i=m; i>0; i--)
     d[i] = d[i-1];
  
  d[0] = a * d[0] + aa * x;

  return (x);
}

double mglsadf1 (double x, double *b, const int m, const double a, const int n, double *d)
{
   int i;
   double g;

   g = -1.0 / (double)n;

   for (i=0; i<n; i++)
      x = mglsadff1(x, b, m, a, g, &d[i*(m+1)]);

   return(x);
}

static double mglsadfft (double x, double *b, const int m, const double a, double *d)
{
   int i;

   x -= d[0] * (1.0 - a * a);

   d[m] = b[m] * x + a * d[m-1];
   for (i=m-1; i>=1; i--)
      d[i] += b[i] * x + a * (d[i-1] - d[i+1]);

   for (i=0; i<m; i++)
      d[i] = d[i+1];

   return (x);
}

double mglsadft (double x, double *b, const int m, const double a, const int n, double *d)
{
   int i;

   for (i=0; i<n; i++)
      x = mglsadfft(x, b, m, a, &d[i*(m+1)]);

   return(x);
}

static double mglsadff1t (double x, double *b, const int m, const double a, const double g, double *d)
{
   int i;

   x -= d[0] * (1.0 - a * a) * g;

   d[m] = b[m] * x + a * d[m-1];
   for (i=m-1; i>=1; i--)
      d[i] += b[i] * x + a * (d[i-1] - d[i+1]);

   for (i=0; i<m; i++)
      d[i] = d[i+1];

   return (x);
}

double mglsadf1t (double x, double *b, const int m, const double a, const int n, double *d)
{
   int i;
   double g;
   
   g = -1.0 / (double)n;

   for (i=0; i<n; i++)
      x = mglsadff1t(x, b, m, a, g, &d[i*(m+1)]);

   return(x);
}
