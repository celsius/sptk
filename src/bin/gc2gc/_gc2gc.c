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

    Generalized Cepstral Transformation	

	void gc2gc(c1, m1, g1, c2, m2, g2)

	double	*c1	: normalized generalized cepstrum (input)
	int	m1	: order of gc1
	double	g1	: gamma of gc1
	double	*c2	: normalized generalized cepstrum (output)
	int	m2	: order of gc2
	double	g2	: gamma of gc2

*****************************************************************/
#include <stdio.h>
#include <SPTK.h>

void gc2gc(c1, m1, g1, c2, m2, g2)
double 	*c1, *c2, g1, g2;
int 	m1, m2;
{
    int 	  i, min, k, mk;
    double 	  ss1, ss2, cc;
    static double *ca = NULL;
    static int    size;

    if(ca == NULL){
        ca = dgetmem(m1+1);
	size = m1;
    }
    if(m1 > size){
        free(ca);
        ca = dgetmem(m1+1);
	size = m1;
    }

    movem(c1, ca, sizeof(*c1), m1+1);

    c2[0] = ca[0];
    for (i = 1; i <= m2; i++){
	ss1 = ss2 = 0.0;
	min = m1 < i ? m1 : i - 1;
	for (k = 1; k <= min; k++){
	    mk = i - k;
	    cc = ca[k] * c2[mk];
	    ss2 += k * cc;
	    ss1 += mk * cc;
	}

	if (i <= m1)
	    c2[i] = ca[i] + (g2*ss2 - g1*ss1)/i;
	else
	    c2[i] = (g2*ss2 - g1*ss1)/i;
    }
}
