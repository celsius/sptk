/****************************************************************
    $Id$

    Solve a Synmetric Toeplitz Set of Linear Equations	

	int toeplitz(t, a, b, n, eps)	< Ta=b >

	double *t  : Toeplitz elements -> T(i,j) = t(|i-j|) t[0]..t[n-1]
	double *a  : solution vector of equation
	double *b  : known vector
	int    n   : system order
	double eps : singular check (eps(if -1., 1.0e-6 is assumed))

	return value : 	0  -> normally completed
			-1 -> abnormally completed

****************************************************************/

#include <stdio.h>
#include <SPTK.h>

int toeplitz(t, a, b, n, eps)
double *t, *a, *b, eps;
int n;
{
    register int 	l, k;
    static double 	*c = NULL, *cc;
    static int		size;
    double 	  	rmd, mue, mue2;
    
    if (c == NULL){
	c = dgetmem(n+n+2);
	cc = c + n;
	size = n;
    }
    if (n > size){
	free(c);
	c = dgetmem(n+n+2);
	cc = c + n;
	size = n;
    }
    
    if (eps < 0.0) eps = 1.0e-6;

    fillz(c, sizeof(*c), n+1);

    rmd = t[0];
    if (((rmd < 0.0) ? -rmd : rmd) <= eps) return(-1);
    
    a[0] = b[0] / rmd;

    for (l=1; l<n; l++){
	mue = -t[l];
	for (k=1; k<l; k++)
	    mue -= c[k] * t[l-k];
	mue /= rmd;

	for (k=1; k<l; k++)
	    cc[k] = c[k] + mue * c[l-k];
	cc[l] = mue;

	rmd = (1.0 - mue*mue) * rmd;
	if (((rmd < 0.0) ? -rmd : rmd) <= eps) return(-1);

	for (k=1; k<=l; k++) c[k] = cc[k];

	mue2 = b[l];
	for (k=0; k<=l-1; k++)
	    mue2 += c[l-k] * b[k];
	mue2 /= rmd;

	for (k=0; k<l; k++)
	    a[k] += mue2 * c[l-k];
	a[l] = mue2;
    }
    return(0);
}
