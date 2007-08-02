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

/************************************************************************
  $Id$

    u-law coder
	
	double ulaw_c(x, max, mu)

	double	  x :  data
	double	max :  max value
	double	 mu :  compression ratio

	return value	: compressed data

    u-law decoder
	
	double ulaw_d(x, max, mu)

	double	  x :  compressed data
	double	max :  max value
	double	 mu :  compression ratio

	return value	: uncompressed data

************************************************************************/

#include <math.h>

#define	abs(x)	((x>=0) ? (x) : (-(x)))
#define	sign(x)	((x>=0) ? 1 : (-1))

double ulaw_c(double x, double max, double mu)
{
    double	y;
    
    y = sign(x) * max * log(1 + mu*abs(x)/max) / log(1 + mu);
    return(y);
}

double ulaw_d(double x, double max, double mu)
{
    double	y, pow(double, double);
    
    y = sign(x) * max * (pow(1+mu, abs(x)/max) -1) / mu;
    return(y);
}

