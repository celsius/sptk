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

/******************************************************
 $Id$
  2nd order standard form digital filter
	double df2(x,sf,f0p,wbp,f0z,wbz,fp,fz,buf,bufp)

	double	x     : filter input
	double	sf    : sampling frequency
	double	f0p   : center frequency of pole
	double	wbp   : band width of pole
	double    f0z   : center frequency of zero
	double    wbz   : band witdth of zero
	int	fp    : if pole is exist then fp = 1 
	int	fz    : if zero is exist then fz = 1
	double	buf[] : buffer ( which require 3 )
	int	*bufp : work pointer
	
	return	      : filter output

		Dec.1995 Naohiro Isshiki
*******************************************************/	 
#include<math.h>

double df2(x,sf,f0p,wbp,f0z,wbz,fp,fz,buf,bufp)
double 	x,sf,f0p,wbp,f0z,wbz;
int	fp,fz;
double	buf[];
int	*bufp;


{
	double a[3], b[3];
	double p, e;
	double dfs();
	
	p = 4 * atan(1.0) / sf;
	e = exp(-p * wbz);

	a[0] = 1.0;
	if(fz){
		a[1] = -2 * e * cos(2 * p * f0z);
		a[2] = e * e;
	}else{
		a[1] = 0;
		a[2] = 0;
	}
		
	e = exp(-p * wbp);
	b[0] = 1.0;
	if(fp){
		b[1] = -2 * e * cos(2 * p * f0p);
		b[2] = e * e;
	} else {
		b[1] = 0;
		b[2] = 0;
	}
	return(dfs(x,b,2,a,2,buf,bufp));
	
}


