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

    $Id: _vq.c,v 1.2 2002/12/25 05:33:10 sako Exp $

    Vector Quantization

	int vq(x, cb, l, cbsize)

	double  *x     : input vector
	double  *cb    : codebook vector
	int     l      : vector order
	int     cbsize : size of codebook

	return value   : codebook index

*****************************************************************/

int vq(x, cb, l, cbsize)
double *x, *cb;
int cbsize, l;
{
    int           i, index;
    double	  min = 1e23, dist, edist();
    
    for(i=0; i<cbsize; i++){
	dist = edist(x, cb, l);
	if(dist < min){
	    index = i;
	    min = dist;
	}
	cb += l;
    }
    return(index);
}

double edist(x, y, m)
double *x, *y;
int m;
{
    register int  i;
    double	  sub, dist = 0.0;
    
    for(i=0; i<m; i++){
	sub = x[i] - y[i];
	dist += sub * sub;
    }
    return(dist / (double)m);
}

