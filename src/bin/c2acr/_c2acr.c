/****************************************************************

    $Id$

    Transformation Cepstrum to Autocorrelation

	void 	c2acr(c, m1, r, m2, flng)

	double    *c : cepstral coefficients
	int   	  m1 : order of cepstrum
	double    *r : autocorrelation coefficients
	int   	  m2 : order of autocorrelation
	int     flng : FFT length

****************************************************************/

#include <stdio.h>
#include <SPTK.h>

void c2acr(c, m1, r, m2, flng)
double *c, *r;
int m1, m2, flng;
{
    register int  i;
    double	  exp();
    static double *x = NULL, *y;
    static int    size;
    
    if(x == NULL){
	x = dgetmem(flng+flng);
	y = x + flng;
	size = flng;
    }
    if(flng > size){
	free(x);
	x = dgetmem(flng+flng);
	y = x + flng;
	size = flng;
    }

    movem(c, x, sizeof(*c), m1+1);
    fillz(&x[m1+1], sizeof(*x), flng-m1-1);

    fftr(x, y, flng);

    for(i=0; i<flng; i++)
	x[i] = exp(2.0 * x[i]);
    
    fftr(x, y, flng);

    for(i=0; i<=m2; i++) 
	r[i] = x[i] / flng;
}
