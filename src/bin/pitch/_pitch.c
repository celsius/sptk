/****************************************************************

    $Id: _pitch.c,v 1.1 2000/03/01 13:58:49 yossie Exp $

    Pitch Extract

   	double pitch(x, m, l, a, thresh, L, H, eps, env);

	double *x     : windowed data sequence
	int    m      : order of cepstrum
	int    l      : frame length
	double a      : pre-emphasis coefficients
	double thresh : voiced/unvoiced threshold
	int    L      : minmum for pitch period
	int    H      : maximum for pitch period
	double eps    : small value for log
	int    env    : order for calculate log-spectral envelope

******************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>

/*  Required Functions  */
int fftr();
double log();


double pitch(xw, m, l, a, thresh, L, H, eps, env)
double *xw, a, thresh, eps;
int m, l, L, H, env;
{
    static double *x = NULL,*y,*z;
    double voiced,max,p,log();
    int i;

    if(x == NULL){
	x = dgetmem(3*l);
	y = x + l;
	z = y + l;
    }

    movem(xw,x,sizeof(*x),l);
    fftr(x, y, l);
    for(i=0; i<l; i++)
	x[i] = log(x[i]*x[i] + y[i]*y[i] + eps);

    movem(x,z,sizeof(*x),l);		/* voiced/unvoiced detection */
    fftr(z,y,l);
    for(i=0;i<=env;i++)
	z[i] /= l;
    fillz(z+env,l-env,sizeof(double));
    fftr(z,y,l);
    voiced = 0.0;
    for(i=4*l/256;i<=17*l/256;i++)
	voiced += z[i];
    voiced /= 14 * l / 256;

    if(voiced > thresh){
	y[0] = x[0] - a * x[l-1];		/* pre-emphasis filter */
	for(i=1; i<l; i++){
	    y[i] = x[i] - a * x[i-1];
	}

	movem(y,x,sizeof(*x),l);
	fftr(x, y, l);
	for(i=0; i<l; i++)
	    x[i] /= l;

	max = 0;
	for(i=L; i<H; i++)
	    if(max < x[i]){
		p = (float)i;
		max = x[i];
	    }
    } else 
	p = 0.0;
    return(p);
}
