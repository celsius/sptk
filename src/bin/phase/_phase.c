/*******************************************************************
  $Id: _phase.c,v 1.1.1.1 2000/03/01 13:58:48 yossie Exp $
	comupte phase of digital filter or real seaquence
		phase(p, mp, z, mz, ph, flng)

		double	*p	: numerator cofficients
		int	mp	: order of numerator polynomial
		double  *z	: denominator cofficients
		int 	mz	: order of denominator polynomilal
		doble 	*ph	: phase 
		int	flng	: FFT size
		int	unlap	: unlapping
			
			Naohiro Isshiki 	Feb.1995
********************************************************************/
#include <stdio.h>
#include <math.h>
#include <SPTK.h>
	
void phase(p, mp, z, mz, ph, flng, unlap)
double 	*p,*z,*ph;
int 	mp, mz, flng, unlap;
{
	static double	*x;
	static int fsize = 0;
	double 	*y, *xx, *yy,*py;
	int	no, i, offset;	
	double pi;

	pi = atan(1.) * 4.;

	no = flng / 2 + 1;

	if (flng > fsize){
		if( x != NULL )
			free(x);			
		fsize = flng;
		x = dgetmem(4 * flng + no);
	}
	y = &x[flng];
	xx = &y[flng];
	yy = &xx[flng];
	py = &yy[flng];

	fillz(x, sizeof(*x), flng);
	fillz(xx, sizeof(*xx), flng);
	movem(z, x, mz+1, sizeof(*z));
	movem(p, xx, mp+1, sizeof(*p));
	
	fftr(x, y, flng);
	xx[0] = 1;
	fftr(xx, yy, flng);
	for (i = 0; i < no; i++){
		ph[i] = x[i] * xx[i] + y[i] * yy[i];
		py[i] = y[i] * xx[i] - x[i] * yy[i];
	}
	offset = 0;
	i = 0;
	ph[i] = atan2(py[i], ph[i]) / pi;
	i++;
	for (; i < no; i++){
		ph[i] = atan2(py[i], ph[i]) / pi;
		if (unlap) {
			if (ph[i - 1] - ph[i] - offset > 1)
				offset += 2;
			else if (ph[i] + offset - ph[i - 1] > 1)
				offset -= 2;
			ph[i] += offset;
		}
	}
}
