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

    $Id: _agcep.c,v 1.3 2006/12/11 07:16:34 mr_alex Exp $

    Adaptive Generalized Cepstral Analysis

	double agcep(x, c, m, lambda, step);

	double   x      : input sequence
	double   *c     : normalized generalized cepstrum
	int      m      : order of generalized cepstrum
	int      stage  : -1 / gamma
	double   lambda : leakage factor
	double   step   : step size
	double   tau	: momentum constant
	double	 eps	: minimum value for epsilon

	return value  : prediction error

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>

double agcep(double x, double *c, int m, int stage, double lambda, double step, double tau, double eps)
{
    register int   i;
    static double  *eg = NULL, *ep, *d, gg = 1.0, ee = 1.0, tx;
    static int     size;
    double 	   mu, ll, iglsadf1(), sqrt();
    
    if(eg == NULL){
	eg = dgetmem(2*(m+1)+m*stage);
	ep = eg + m + 1;
	d = ep + m + 1;
	size = m;
    }
    if(m > size){
	free(eg);
	eg = dgetmem(2*(m+1)+m*stage);
	ep = eg + m + 1;
	d = ep + m + 1;
	size = m;
    }

    ll = 1.0 - lambda;
    
    eg[m] = d[stage*m-1];
    x = iglsadf1(x, c, m, stage, d);
    
    movem(d+(stage-1)*m, eg, sizeof(*d), m);
    
    gg = lambda * gg + ll * eg[0] * eg[0];
    gg = ( gg < eps )? eps: gg;
    mu = step / (double) m /gg;
    tx = 2 * (1.0 - tau) * x;
    
    for(i=1; i<=m; i++){
	ep[i] = tau * ep[i] - tx * eg[i];
	c[i] -= mu * ep[i];
    }
    
    ee = lambda * ee + ll * x * x;
    c[0] = sqrt(ee);

    return(x);
}
