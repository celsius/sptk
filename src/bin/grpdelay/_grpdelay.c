/***************************************************************
  $Id: _grpdelay.c,v 1.1.1.1 2000/03/01 13:58:35 yossie Exp $
	group delay of digital filter
		grpdelay(x, gd, size, is_alma);

		double	*x:	 filter coefficients  seaqunece
		double	*gd:	 group delay sequence
		int	size:	 FFT size		
		int     is_arma: if a then ARMA filter 

		Naohiro Isshiki		Feb. 1996
****************************************************************/
#include <stdio.h>
#include <SPTK.h>

void grpdelay(x,gd,size,is_arma)
double	*x, *gd;
int	size, is_arma;

{
	static double *y;
	static int    fsize;
	
	double	*u, *v;
	register int	k, size_2;

	if ( fsize < size ){
		if ( y != NULL )
			free(y);
		fsize = size;
		y = dgetmem(3*size);
	}
	movem(x,gd,sizeof(*x),size);
	u = y + size;
	v = u + size;

	size_2 = size / 2;

	if(is_arma)
		gd[0] = 1;
	for(k = 0; k < size; ++k)
		u[k] = gd[k] * k;
	fftr(gd, y, size);
	fftr(u, v, size);
	for(k = 0; k <= size_2; ++k) {
		gd[k] = (gd[k] * u[k] + y[k] * v[k]) /
			(gd[k] * gd[k] + y[k] * y[k]);
		if(is_arma)
			gd[k] *= -1;
	}
}
