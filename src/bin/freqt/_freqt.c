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

    $Id: _freqt.c,v 1.6 2007/08/07 04:20:26 heigazen Exp $

    Frequency Transformation

	void	freqt(c1, m1, c2, m2, a)

	double	*c1   : minimum phase sequence
	int	m1  : order of minimum phase sequence
	double	*c2   : warped sequence
	int	m2  : order of warped sequence
	double	a     : all-pass constant

***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>

void freqt (double *c1, const int m1, double *c2, const int m2, const double a)
{
   int i, j;
   double b;
   static double *d=NULL, *g;
   static int size;
    
   if (d==NULL) {
      size = m2;
      d = dgetmem(size+size+2);
      g = d + size + 1;
   }

   if (m2>size) {
      free(d);
      size = m2;
      d = dgetmem(size+size+2);
      g = d + size + 1;
   }
    
   b = 1 - a*a;
   fillz(g, sizeof(*g), m2+1);

   for (i=-m1; i<=0; i++) {
      if (0<=m2)
         g[0] = c1[-i] + a*(d[0] = g[0]);
      if (1<=m2)
         g[1] = b*d[0] + a*(d[1] = g[1]);
      for (j=2; j<=m2; j++)
         g[j] = d[j-1] + a*((d[j]=g[j]) - g[j-1]);
   }
    
   movem(g, c2, sizeof(*g), m2+1);

   return;
}

