/****************************************************************

    $Id$

    Adaptive Mel Cepstral Analysis

	double amcep(x, b, m, a, lambda, step, tau, pd, eps);

	double   x      : input sequence
	double   *b     : MLSA filter coefficients
	int      m      : order of cepstrum
	double   a 	: alpha
	double   lambda : leakage factor
	double   step   : step size
	double   tau    : momentum constant
	int      pd     : order of pade approximation
	double   eps	: minimum value for epsilon

	return value  : prediction error

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>


double amcep(x, b, m, a, lambda, step, tau, pd, eps)
double x, *b, a, lambda, step, tau, eps;
int m, pd;
{
    register int   i;
    static double  *bb = NULL, *d, *ep, *e, xx, gg = 1.0;
    static int     size;
    double 	   tt, mu, tx, mlsadf(), log();
    
    if(bb == NULL){
	bb = dgetmem(3*(m+1)+3*(pd+1)+pd*(m+2));
	e  = bb + m + 1;
	ep = e  + m + 1;
	d  = ep + m + 1;
	size = m;
    }
    if(m > size){
	free(bb);
	bb = dgetmem(3*(m+1)+3*(pd+1)+pd*(m+2));
	e  = bb + m + 1;
	ep = e  + m + 1;
	d  = ep + m + 1;
	size = m;
    }
	
    for(i=1; i<=m; i++)
	bb[i] = -b[i];
    
    x = mlsadf(x, bb, m, a, pd, d);
    phidf(xx, m, a, e);
    xx = x;

    gg = gg * lambda + (1.0 - lambda) * x * x;
    gg = ( gg < eps )? eps : gg;
    b[0] = 0.5 * log(gg);

    mu = step / (double) m / gg;
    tx = 2 * (1.0 - tau) * x;
    
    for(i=1; i<=m; i++){
	ep[i] = tau * ep[i] - tx * e[i];
	b[i] -= mu * ep[i];
    }
    
    return(x);
}

phidf(x, m, a, d)
double x, a, *d;
int m;
{
    register int   i;

    d[0] = a * d[0] + (1.0 - a*a) * x;    
    for(i=1; i<m; i++)
	d[i] += a * (d[i+1] - d[i-1]);

    for(i=m; i>=1; i--) 
	d[i] = d[i-1];
}
