/****************************************************************

    $Id$

    Generalized Cepstral Analysis

	int gcep(xw, flng, gc, m, g, itr1, itr2, d, e);

	double  *xw  : input sequence
	int    	flng : frame length
	double  *gc  : generalized cepstrum
	int    	m    : order of generalized cepstrum
	double 	g    : gamma
	int    	itr1 : minimum number of iteration
	int    	itr2 : maximum number of iteration
	double 	d    : end condition
	double 	e    : initial value for log-periodgram

	return value : 	0 -> completed by end condition
			-1-> completed by maximum iteration

*****************************************************************/

#include	<stdio.h>
#include	<SPTK.h>

int gcep(xw, flng, gc, m, g, itr1, itr2, d, e)
double *xw, *gc, d, g, e;
int flng, itr1, itr2, m;
{
    int		  i, j, flag = 0;
    double	  t, s, dd, fabs(), exp(), log(), sqrt(), agexp();
    static double *x = NULL, *y, *cr, *ci, *rr, *hr, *hi, *er, *ei;
    static int    size;

    if(x == NULL){
	x = dgetmem(9*flng);
	size = flng;

	y =  x  + flng;	cr = y  + flng;
	ci = cr + flng;	rr = ci + flng;
	hr = rr + flng;	hi = hr + flng;
	er = hi + flng;	ei = er + flng;
    }
    if(flng > size){
	free(x);
	x = dgetmem(9*flng);
	size = flng;

	y =  x  + flng;	cr = y  + flng;
	ci = cr + flng;	rr = ci + flng;
	hr = rr + flng;	hi = hr + flng;
	er = hi + flng;	ei = er + flng;
    }

    movem(xw, x, sizeof(*x), flng);
    
    /*  power spectrum  */
    fftr(x, y, flng);
    for(i=0; i<flng; i++){
	x[i] = x[i]*x[i] + y[i]*y[i];
	cr[i] = log(x[i] + e);
    }
    

    /*  initial value of generalized cepstrum  */
    ifftr(cr, y, flng);				/*  x : IFFT[x]  */
    cr[0] = exp(cr[0] / 2);
    gc2gc(cr, m, 0.0, gc, m, g); 	/*  gc : gen. cep.  */

    /*  Newton-Raphson method  */
    for (j=1; j<=itr2; j++){
	fillz(cr, sizeof(*cr), flng);
	movem(&gc[1], &cr[1], sizeof(*cr), m);

	fftr(cr, ci, flng);			/*  cr+jci : FFT[gc]  */
	for (i = 0; i < flng; i++){
	    t = x[i] / agexp(g, cr[i], ci[i]);
	    cr[i] = 1 + g * cr[i];
	    ci[i] = g * ci[i];
	    s = cr[i] * cr[i] + ci[i] * ci[i];
	    rr[i] = t / s;
	    hr[i] = (cr[i] * cr[i] - ci[i] * ci[i]) * t / (s * s);
	    hi[i] = 2 * cr[i] * ci[i] * t / (s * s);
	    er[i] = cr[i] * t / s;
	    ei[i] = ci[i] * t / s;
	}
	
	ifftr(rr, y, flng);			/*  rr : r(k)  */
	ifft(hr, hi, flng);			/*  hr : h(k)  */
	ifft(er, ei, flng);			/*  er : e(k)  */
	s = gc[0];				/*  gc[0] : gain  */
	for (i=1,t=0.0; i <=m; i++)
	    t += er[i] * gc[i];
	t = er[0] + g * t;
	t = sqrt(fabs(t));

	if(j >= itr1){
	    if (fabs((t - dd)/t) < d){
		 flag = 1;
		 break;
	    }
	    dd = t;
	}

	for (i=2; i<=m+m; i++) hr[i] *= 1 + g;

	if (theq(rr, &hr[2], &y[1], &er[1], m, -1.0)){
	    fprintf(stderr, "gcep : Error in theq() at %dth iteration !\n", j);
	    exit(1);
	}

	gc[0] = t;
	for (i=1; i<=m; i++) gc[i] += y[i];
    }

    if(flag) return(0);
    else     return(-1);
}
