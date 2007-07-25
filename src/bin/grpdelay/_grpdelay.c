/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/***************************************************************
  $Id$
	group delay of digital filter
		grpdelay(x, gd, size, is_alma);

		double	*x:	 filter coefficients  seaqunece
		double	*gd:	 group delay sequence
		int	size:	 FFT size		
		int     is_arma: if a then ARMA filter 

		Naohiro Isshiki		Feb. 1996
****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>

void grpdelay (double *x, double *gd, const int size, const int is_arma)
{
   static double *y;
   static int fsize;
   
   double *u, *v;
   int k, size_2;
   
   if (fsize<size) {
      if (y!=NULL)
         free(y);
      fsize = size;
      y = dgetmem(3*size);
   }
   movem(x,gd,sizeof(*x),size);
   u = y + size;
   v = u + size;

   size_2 = size / 2;

   if(is_arma)
      gd[0] = 1;
   for (k=0; k<size; ++k)
      u[k] = gd[k] * k;
   
   fftr(gd, y, size);
   fftr(u, v, size);
   
   for (k=0; k<=size_2; k++) {
      gd[k] = (gd[k]*u[k]+y[k]*v[k]) / (gd[k]*gd[k]+y[k]*y[k]);
      if (is_arma)
         gd[k] *= -1;
   }
}
