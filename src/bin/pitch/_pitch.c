/**********************************************************************

    $Id: _pitch.c,v 1.3 2000/04/06 08:57:57 mtamura Exp $

    Pitch Extract

   	double pitch(xw, freq, l, a, thresh, L, H, eps, m1, m2, alpha);

	double *xw    : windowed data sequence
	int    freq   : sampling frequency
	int    l      : frame length (fft size)
	double a      : pre-emphasis coefficients
	double thresh : voiced/unvoiced threshold
	int    L      : minmum fundamental frequency to search for
	int    H      : maximum fundamental frequency to search for
	double eps    : small value for log
	int    m1     : order of LPC
	int    m2     : order of mel cepstrum
	double alpha  : all-pass constant

************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>

/*  Required Functions  */
int fftr();
int lpc();
void ignorm();
void mgc2mgc();
double log();


double pitch(xw, freq, l, a, thresh, L, H, eps, m1, m2, alpha)
double *xw, a, thresh, eps, alpha;
int freq, l, L, H, m1, m2;
{
    static double *x = NULL,*y,*z, *c;
    double voiced,max,p,log();
    int i, low, high;

    if(x == NULL){
	x = dgetmem(3*l+m1+1);
	y = x + l;
	z = y + l;
	c = z + l;
    }

    movem(xw,x,sizeof(*x),l);

/* voiced/unvoiced detection */
    lpc(x, l, c, m1);
    ignorm(c, c, m1, -1.0);
    for(i = m1;i >= 1; i--) c[i] /= -1.0;
    mgc2mgc(c, m1, 0.0, -1.0, z, m2, alpha, 0.0);
    fillz(z+m2,l-m2,sizeof(double));
    fftr(z,y,l);

    voiced = 0.0;
    for(i=4*l/256;i<=17*l/256;i++)
	voiced += z[i];
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

	max = 0;
	low = freq * 1000 / H;
	high = freq * 1000 / L;

	for(i=low; i<high; i++)
	    if(max < x[i]){
		p = (float)i;
		max = x[i];
	    }
    } else 
	p = 0.0;
    return(p);
}
