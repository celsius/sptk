/****************************************************************

    $Id$

    Solve an Autocorrelation Normal Equation
			Using Levinson-Durbin Method

   	int levdur(r, a, m, eps);

	double  *r : autocorrelation sequence
	double  *a : LP coefficients
	int     m  : order of LPC
	double eps : singular check (eps(if -1., 1.0e-6 is assumed))

	return value : 	0  -> normally completed
			-1 -> abnormally completed
			-2 -> unstable LPC

******************************************************************/

#include <stdio.h>
#include <SPTK.h>

int levdur(r, a, m, eps)
double *r, *a, eps;
int m;
{
    int 		l, k, flag=0;
    double 		rmd, mue, sqrt();
    static double 	*c = NULL;
    static int		size;

    if(c == NULL){
	c = dgetmem(m+1);
	size = m;
    }

    if(m > size){
	free(c); 
	c = dgetmem(m+1);
	size = m;
    }
    
    if (eps < 0.0) eps = 1.0e-6;

    rmd = r[0];
    a[0] = 0.0;

    for (l = 1; l <= m; l++){
	mue = -r[l];
	for (k = 1; k < l; k++)
	    mue -= c[k] * r[l - k];
	mue = mue / rmd;

	for (k = 1; k < l; k++)
	    a[k] = c[k] + mue * c[l - k];
	a[l] = mue;

	rmd = (1.0 - mue * mue) * rmd;
	if (((rmd < 0.0) ? -rmd : rmd) <= eps) return(-1);
	if (((mue < 0.0) ? -mue : mue) >= 1.0) flag = -2;

	for (k = 0; k <= l; k++) c[k] = a[k];
    }
    a[0] = sqrt(rmd);
    return(flag);
}
