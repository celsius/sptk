/****************************************************************

    $Id: _agcep.c,v 1.1.1.1 2000/03/01 13:58:28 yossie Exp $

    Adaptive Generalized Cepstral Analysis

	double agcep(x, c, m, lambda, step);

	double   x      : input sequence
	double   *c     : normalized generalized cepstrum
	int      m      : order of generalized cepstrum
	int      stage  : -1 / gamma
	double   lambda : leakage factor
	double   step   : step size
	double   tau	: momentum constant
	double	 eps	: minimum value for epsilon

	return value  : prediction error

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>

double agcep(x, c, m, stage, lambda, step, tau, eps)
double x, *c, lambda, step, tau, eps;
int m, stage;
{
    register int   i;
    static double  *eg = NULL, *ep, *d, gg = 1.0, ee = 1.0, tx;
    static int     size;
    double 	   mu, ll, iglsadf1(), sqrt();
    
    if(eg == NULL){
	eg = dgetmem(2*(m+1)+m*stage);
	ep = eg + m + 1;
	d = ep + m + 1;
	size = m;
    }
    if(m > size){
	free(eg);
	eg = dgetmem(2*(m+1)+m*stage);
	ep = eg + m + 1;
	d = ep + m + 1;
	size = m;
    }

    ll = 1.0 - lambda;
    
    eg[m] = d[stage*m-1];
    x = iglsadf1(x, c, m, stage, d);
    
    movem(d+(stage-1)*m, eg, sizeof(*d), m);
    
    gg = lambda * gg + ll * eg[0] * eg[0];
    gg = ( gg < eps )? eps: gg;
    mu = step / (double) m /gg;
    tx = 2 * (1.0 - tau) * x;
    
    for(i=1; i<=m; i++){
	ep[i] = tau * ep[i] - tx * eg[i];
	c[i] -= mu * ep[i];
    }
    
    ee = lambda * ee + ll * x * x;
    c[0] = sqrt(ee);

    return(x);
}
