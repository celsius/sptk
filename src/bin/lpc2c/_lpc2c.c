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

    $Id: _lpc2c.c,v 1.4 2007/07/23 02:28:03 mr_alex Exp $

    Transformation LPC to Cepstrum 

	void 	lpc2c(a, m1, c, m2)

	double  *a : LP coefficients
	int   	m1 : order of LPC
	double  *c : cepstral coefficients
	int   	m2 : order of cepstrum

****************************************************************/

void lpc2c(double *a, int m1, double *c, int m2)
{
    int 	i, k, upl;
    double	 	log(double), exp(double), d;

    c[0] = log(a[0]);
    c[1] = - a[1];
    for(k = 2; k <= m2; ++k){
	upl = (k > m2) ? m2+1 : k;
	
	for(d=0.0, i=(k>m1) ? k-m1 : 1; i<upl; i++)
	    d += i * c[i] * a[k-i];
	c[k] = -d / k;
	
	if(k <= m1) c[k] -= a[k];
    }
}

