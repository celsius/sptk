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

/****************************************************************

    $Id: _fftcep.c,v 1.2 2002/12/25 05:29:31 sako Exp $

    FFT Cepstral Analysis

   	void fftcep(sp, flng, c, m, itr, ac);

	double *sp  : log power spectrum
	int    flng : frame length
	double *c   : cepstral coefficients
	int    m    : order of cepstrum
	int    itr  : number of iteration
	double ac   : accelation factor

******************************************************************/

#include <stdio.h>
#include <SPTK.h>

void fftcep(sp, flng, c, m, itr, ac)
double *sp, *c, ac;
int flng, m, itr;
{
    double  	   temp;
    static double  *x = NULL, *y;
    register int   k, size;

    if(x == NULL){
	x = dgetmem(flng+flng);
	y = x + flng;
	size = flng;
    }
    if(flng > size){
	free(x);
	x = dgetmem(flng+flng);
	y = x + flng;
	size = flng;
    }

    movem(sp, x, sizeof(*sp), flng);
    
    fftr(x, y, flng);
    for(k=0; k<flng; k++) x[k] /= flng;
    for(k=0; k<=m; k++){
	c[k] = x[k];
	x[k] = 0;
    }

    ac += 1.0;
    while(--itr > 0){
	for(k=1; k<=m; k++)  
	    x[flng-k] = x[k];

	fftr(x, y, flng);

	for(k=0; k<flng; k++)
	    if(x[k] < 0.) x[k] = 0.;
	    else	  x[k] /= flng;

	fftr(x, y, flng);

	for(k=0; k<=m; k++){
	    temp = x[k] * ac;
	    c[k] += temp;
	    x[k] -= temp;
	}
    }
    c[0] *= 0.5;

    if(m == flng/2)
	c[m] *= 0.5;
}
