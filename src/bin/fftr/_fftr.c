/********************************************************
 $Id: _fftr.c,v 1.1 2000/03/01 13:58:29 yossie Exp $							
 NAME:							
	fftr - Fast Fourier Transform for Double sequence	   
 SYNOPSIS:                                             
	int	fftr(x, y, m)				
							
	double	x[];	real part of data		
	double	y[];	working area			
	int	m;	number of data(radix 2)		
		Naohiro Isshiki 	Dec.1995	modified
********************************************************/
#include <stdio.h>
#include <math.h>
#include <SPTK.h>

extern double	*_sintbl;
extern int	maxfftsize;
int fftr(x, y, m)
double	*x, *y;
int	m;
{
	register int	i, j ,k;
	register double	*xp, *yp, *xq;
	double		*yq;
	int		mv2, n, tblsize;
	double		xt, yt, *sinp, *cosp;
	double		arg;
	mv2 = m / 2;

	/* separate even and odd  */
	xq = xp = x;
	yp = y;
	for(i = mv2; --i >= 0; ) {
		*xp++ = *xq++;
		*yp++ = *xq++;
	}

	if(fft(x, y, mv2) == -1)        /* m / 2 point fft */
		return(-1);


	/***********************
	* SIN table generation *
	***********************/

	if(_sintbl == 0 || maxfftsize < m ) {
		tblsize=m-m/4+1;
		arg=PI/m*2;
		if (_sintbl != 0)
			free(_sintbl);
		_sintbl = sinp = dgetmem(tblsize);
		*sinp++ = 0;
		for(j = 1 ; j < tblsize ; j++)
			*sinp++ = sin( arg * (double)j);
		_sintbl[m/2] = 0;
		maxfftsize = m;
	}

	n = maxfftsize / m;
	sinp = _sintbl;
	cosp = _sintbl + maxfftsize/4;

	xp = x;
	yp = y;
	xq = xp + m;
	yq = yp + m;
	*(xp + mv2) = *xp - *yp;
	*xp = *xp + *yp;
	*(yp + mv2) = *yp = 0;
	for(i = mv2, j = mv2 - 2; --i ; j -= 2) {
		++xp;
		++yp;
		sinp += n;
		cosp += n;
		yt = *yp + *(yp + j);
		xt = *xp - *(xp + j);
		*(--xq) = (*xp + *(xp + j) + *cosp * yt - *sinp * xt) * 0.5;
		*(--yq) = (*(yp + j) - *yp + *sinp * yt + *cosp * xt) * 0.5;
	}

	xp = x + 1;
	yp = y + 1;
	xq = x + m;
	yq = y + m;
	for(i = mv2; --i; ) {
		*xp++ =   *(--xq); 
		*yp++ = -(*(--yq));
	}
	return(0);
}
