/****************************************************************

    $Id: _acep.c,v 1.1 2000/03/01 13:58:26 yossie Exp $

    Adaptive Cepstral Analysis

	double acep(x, c, m, lambda, step, pd);

	double   x      : input sequence
	double   *c     : cepstrum
	int      m      : order of cepstrum
	double   lambda : leakage factor
	double   step   : step size
	int	 pd	: order of pade approximation
	double   eps	: minimum value for epsilon

	return value  : prediction error

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>

#define PADEORD 	4

double acep(x, c, m, lambda, step, tau, pd, eps)
double x, *c, lambda, step, eps;
int m, pd;
{
    register int   i;
    static double  *cc = NULL, *e, *ep, *d, gg = 1.0;
    static int     size;
    double 	   mu, tx, lmadf(), log();
    
    if(cc == NULL){
	cc = dgetmem(m+m+m+3+(m+1)*PADEORD*2);
	e  = cc + m + 1;
	ep = e + m + 1;
	d  = ep + m + 1;
	size = m;
    }
    if(m > size){
	free(cc);
	cc = dgetmem(m+m+m+3+(m+1)*PADEORD*2);
	e  = cc + m + 1;
	ep = e + m + 1;
	d  = ep + m + 1;
	size = m;
    }

    for(i=1; i<=m; i++)
	cc[i] = -c[i];
    
    x = lmadf(x, cc, m, pd, d);

    for(i=m; i>=1; i--)
	e[i] = e[i-1];
    e[0] = x;
    
    gg = gg * lambda + (1.0 - lambda) * e[0] * e[0];
    c[0] = 0.5 * log(gg);
    
    gg = ( gg < eps )? eps : gg;
    mu = step / (double) m / gg;
    tx = 2 * (1.0 - tau) * x;
    
    for(i=1; i<=m; i++){
	ep[i] = tau * ep[i] - tx * e[i];
	c[i] -= mu * ep[i];
    }
    
    return(x);
}
