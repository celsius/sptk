/********************************************************
*	ifft2 : two dimensional inverse fast Fourier	*
*			transform			*
*							*
*	int ifft2( x, y, n )				*
*							*
*	real	x[0]...x[n*n-1]; real part		*
*	real	y[0]...y[n*n-1]; imaginary part		*
*	int	n;	size of IDFT			*
*							*
*	T. Kobayashi	May, 1989.			*
********************************************************/

#include <stdio.h>
#include <SPTK.h>

int	ifft2( x, y, n )
double	x[], y[];
int	n;
{
	double		*calloc(), *xq, *yq;
	static double	*xb = NULL, *yb;
	register double	*xp, *yp;
	register int	i, j;
	static int	size_f;
	int		ifft();

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
		
		if( ifft( xb, yb, n ) < 0 )
			return( -1 );

		xp = xb;	xq = x + i;
		yp = yb;	yq = y + i;
		for(j = n; --j >= 0; xq += n, yq += n) {
			*xq = *xp++;
			*yq = *yp++;
		}
	}

	for(i = n, xp = x, yp = y ; --i >= 0; xp += n, yp += n) {
		if( ifft( xp, yp, n ) < 0 )
			return( -1 );
	}

	return( 0 );
}
