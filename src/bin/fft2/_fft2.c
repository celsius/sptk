/********************************************************
$Id: _fft2.c,v 1.1 2000/03/01 13:58:34 yossie Exp $

*	fft2 : two dimensional fast Fourier transform 	*
*			for complex sequence		*
*							*
*	int fft2( x, y, n )				*
*							*
*	double	x[0]...x[n*n-1];			*
*			input : data sequence		*
*			output : real part of DFT	*
*	double	y[0]...y[n*n-1];			*
*			input : working area		*
*			output : imaginary part of DFT	*
*	int	n;	size of DFT			*
*							*
*	T. Kobayashi	May, 1989.			*
********************************************************/
#include <stdio.h>
#include <math.h>
#include <SPTK.h>

int	fft2( x, y, n )
double	x[], y[];
int	n;
{
	double		*xq, *yq;
	static double	*xb = NULL, *yb;
	register double	*xp, *yp;
	register int	i, j;
	static int size_f;

	if(xb == NULL) {
		size_f = 2 * n;
		xb = dgetmem(size_f);
		yb = xb + n;
	}
	if (2*n > size_f) {
		free(xb);
		size_f = 2 * n;
		xb = dgetmem(size_f);
		yb = xb + n;
	}
	
	for(i = 0; i < n; ++i) {
		xp = xb;	xq = x + i;
		yp = yb;	yq = y + i;
		for(j = n; --j >= 0; xq += n, yq += n) {
			*xp++ = *xq;
			*yp++ = *yq;
		}
		
		if( fft( xb, yb, n ) < 0 )
			return( -1 );

		xp = xb;	xq = x + i;
		yp = yb;	yq = y + i;
		for(j = n; --j >= 0; xq += n, yq += n) {
			*xq = *xp++;
			*yq = *yp++;
		}
	}

	for(i = n, xp = x, yp = y ; --i >= 0; xp += n, yp += n) {
		if( fft( xp, yp, n ) < 0 )
			return( -1 );
	}

	return( 0 );
}
