/****************************************************************

    $Id: _icep.c,v 1.1 2000/03/01 13:58:36 yossie Exp $

    Imporved Cepstral Analysis

   	void icep(sp, flng, c, m, itr, ac);

	double *sp  : log power spectrum
	int    flng : frame length
	double *c   : cepstral coefficients
	int    m    : order of cepstrum
	int    itr  : number of iteration
	double ac   : accelation factor

******************************************************************/

#include <stdio.h>
#include <SPTK.h>

void icep(sp, flng, c, m, itr, ac)
double *sp, *c, ac;
int flng, m, itr;
{
    double  	   temp;
    static double  *x = NULL, *y;
    register int   k, size;

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

    movem(sp, x, sizeof(*sp), flng);
    
    fftr(x, y, flng);
    for(k=0; k<flng; k++) x[k] /= flng;
    for(k=0; k<=m; k++){
	c[k] = x[k];
	x[k] = 0;
    }

    ac += 1.0;
    while(--itr > 0){
	for(k=1; k<=m; k++)  
	    x[flng-k] = x[k];

	fftr(x, y, flng);

	for(k=0; k<flng; k++)
	    if(x[k] < 0.) x[k] = 0.;
	    else	  x[k] /= flng;

	fftr(x, y, flng);

	for(k=0; k<=m; k++){
	    temp = x[k] * ac;
	    c[k] += temp;
	    x[k] -= temp;
	}
    }
    c[0] *= 0.5;

    if(m == flng/2)
	c[m] *= 0.5;
}
