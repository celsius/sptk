/***********************************************************************

    $Id: _mgc2sp.c,v 1.1 2000/03/01 13:58:42 yossie Exp $

    Transformation mel-generalized cepstrum to log spectrum

	void mgc2sp(mgc, m, a, g, sp, flng)

	double	*mgc 	: mel-generalize cepstrum
	int	m    	: order of mel-generalized cepstrum
	double	a    	: alpha
	double	g    	: gamma
	double	*x  	: real part of spectrum
	double	*y  	: imaginary part of spectrum
	int 	flng 	: FFT length

***********************************************************************/

#include <stdio.h>
#include <SPTK.h>

void mgc2sp(mgc, m, a, g, x, y, flng)
double *mgc, *x, *y, a, g;
int m, flng;
{
    static double *c = NULL;
    static int  size;
    
    if(c == NULL){
	c = dgetmem(flng/2 + 1);
	size = flng;
    }
    if(flng > size){
	free(c);
	c = dgetmem(flng/2 + 1);
	size = flng;
    }
    
    mgc2mgc(mgc, m, a, g, c, flng/2, 0.0, 0.0);
    c2sp(c, flng/2, x, y, flng);
}
