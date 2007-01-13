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
  $Id: _nrand.c,v 1.4 2007/01/13 06:06:36 s_sako Exp $

    Generate Normal Distributed Random Value
        nrand(p, leng, seed)

        double *p     :  generated normal distributed random value
        int    leng   :  length
        int    seed   :  seed of random value

****************************************************************/

#include	<stdio.h>
#include	<math.h>
#include	<SPTK.h>


#define	RAND_MAX	32767

double nrandom(unsigned long *next);
double rnd(unsigned long *next);
unsigned long srnd( unsigned seed);

int nrand(double *p, int leng, int seed)
{
    int i;
    unsigned long next;

	if (seed != 1)
		next = srnd((unsigned)seed);
	for (i=0;i<leng;i++)
		p[i] = (double)nrandom(&next);

	return(0);
}

double nrandom(unsigned long *next)
{
	static int	sw = 0;
	static double	r1, r2, s;

	if (sw == 0)  {
		sw = 1;
		do  {
			r1 = 2 * rnd(next) - 1;
			r2 = 2 * rnd(next) - 1;
			s = r1 * r1 + r2 * r2;
		}  while (s > 1 || s == 0);
		s = sqrt(-2 * log(s) / s);
		return ( r1 * s );
	}
	else  {
		sw = 0;
		return ( r2 * s );
	}
}

double rnd(unsigned long *next)
{
	double	r;

	*next = *next * 1103515245L + 12345;
	r = (*next / 65536L) % 32768L;

	return ( r / RAND_MAX ); 
}

unsigned long srnd( unsigned seed )
{
	return(seed);
}
