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

    $Id$

    Decoder of Multi Stage Vector Quantization

	imsvq(index, cb, l, cbsize, stage, x)

	int    *index  : index of codebook
	real   *cb     : codebook vector
	int    l       : order of vector
	int    *cbsize : size of codebook
	int    stage   : number of stage
	real   *x      : decoded vector

*****************************************************************/

#include	<stdio.h>
#include	<SPTK.h>

void imsvq(int *index, double *cb, int l, int *cbsize, int stage, double *x)
{
    register int   i, j;
    double	   *p;
    static double  *xx = NULL;
    static int     size;
    
    if(xx == NULL){
	xx = dgetmem(l);
	size = l;
    }
    if(size > l){
	free(xx);
	xx = dgetmem(l);
	size = l;
    }

    fillz(x, sizeof(*x), l);

    for(i=0; i<stage; i++){
	ivq(index[i], cb, l, xx);

	for(j=0; j<l; j++)
	    x[j] += xx[j];

	cb += cbsize[i] * l;
    }
}
