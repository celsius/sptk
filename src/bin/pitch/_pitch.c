/**********************************************************************

    $Id: _pitch.c,v 1.4 2000/05/19 07:22:45 mtamura Exp $

    Pitch Extract

        double pitch(xw, l, a, thresh, low, high, eps, m, itr1, itr2, end)

	double *xw    : windowed data sequence
	int    l      : frame length (fft size)
	double a      : pre-emphasis coefficients
	double thresh : voiced/unvoiced threshold
	int    low    : minmum points to search for
	int    high   : maximum points to search for
	double eps    : small value for log
	int    m      : order of cepstrum
	int    itr1   : minimum number of iteration
	int    itr2   : maximum number of iteration
	double end    : end condition

************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>

/*  Required Functions  */
int fftr();
int uels();
double log();


double pitch(xw, l, a, thresh, low, high, eps, m, itr1, itr2, end)
double *xw, a, thresh, eps, end;
int l, low, high, m, itr1, itr2;
{
    static double *x = NULL,*y, *c;
    double voiced,max,p,log();
    int i;

    if(x == NULL){
	x = dgetmem(3*l);
	y = x + l;
	c = y + l;
    }

    movem(xw,x,sizeof(*x),l);

/* voiced/unvoiced detection */
    uels(x, l, c, m, itr1, itr2, end, eps); 
    fillz(c+m,l-m,sizeof(double));
    fftr(c,y,l);

    voiced = 0.0;
    for(i=4*l/256;i<=17*l/256;i++)
	voiced += c[i];
    voiced /= 14 * l / 256;

    fftr(x, y, l);
    for(i=0; i<l; i++)
	x[i] = log(x[i]*x[i] + y[i]*y[i] + eps);

    if(voiced > thresh){
	y[0] = x[0] - a * x[l-1];		/* pre-emphasis filter */
	for(i=1; i<l; i++){
	    y[i] = x[i] - a * x[i-1];
	}

	movem(y,x,sizeof(*x),l);
	fftr(x, y, l);
	for(i=0; i<l; i++)
	    x[i] /= l;

	max = 0.0;
	for(i=low; i<high; i++)
	    if(max < x[i]){
		p = (float)i;
		max = x[i];
	    }
    } else 
	p = 0.0;
    return(p);
}
