/****************************************************************
    $Id: cholesky.c,v 1.1.1.1 2000/03/01 13:58:55 yossie Exp $

    Solve Linear Set of Equations
		       Using Cholesky Decomposition 

	int	cholesky(c, a, b, n, eps)	< Ca=b >

	double	*c  : symmetrical coefficient matrix
	double	*a  : solution vector
	double	*b  : constant vector
	int	n   : order of vector
	double  eps : error check (if -1.0, 1.0e-6 is assumed)

	return value : 	0  -> normally completed
			-1 -> abnormally completed

****************************************************************/

#include <stdio.h>
#include <SPTK.h>

int cholesky(c, a, b, n, eps)
double	*c, *b, *a, eps;
int	n;
{
    register int	i, j, k;
    static double	*d = NULL, *y, *v, *vp;
    static int		size;

    if(d == NULL){
	d = dgetmem(n*(n+2));
	y = d + n;
	v = y + n;
	size = n;
    }

    if(n > size){
	free(d);
	d = dgetmem(n*(n+2));
	y = d + n;
	v = y + n;
	size = n;
    }

    if(eps < 0.0) eps = 1.0e-6;
    
    for(j=0; j<n; j++, c+=n){
	d[j] = c[j];
	vp = v + j*n;
	for(k = 0; k < j; ++k)
	    d[j] -= vp[k] * vp[k] * d[k];

	if(ABS(d[j]) <= eps) return(-1);

	for(i=j+1; i<n; i++){
	    vp = v + i*n;
	    vp[j] = c[i];
	    for(k=0; k<j; ++k)
		vp[j] -= vp[k] * v[j*n + k] * d[k];
	    vp[j] /= d[j];
	}
    }

    for(i=0; i<n; i++){
	y[i] = b[i];
	vp = v + i*n;
	for(k=0; k<i; k++)
	    y[i] -= vp[k] * y[k];
    }

    for(i=n-1; i>=0; i--){
	a[i] = y[i] / d[i];
	for(k=i+1; k<n; k++)
	    a[i] -= v[n*k + i] * a[k];
    }
    return(0);
}


