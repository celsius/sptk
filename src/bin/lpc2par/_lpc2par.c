/***************************************************************

    $Id: _lpc2par.c,v 1.1.1.1 2000/03/01 13:58:40 yossie Exp $

    Transformation LPC to PARCOR 

	int lpc2par(a, k, m)

	double  *a : LP coefficients	
	double  *k : PARCOR coefficients
	int      m : order of LPC

	return value : 	0  -> normally completed
			-1 -> abnormally completed

****************************************************************/

#include <stdio.h>
#include <SPTK.h>

int lpc2par(a, k, m)
double *a, *k;
int  m;
{
    int 	  i, n, flg = 0;
    double  	  s;
    static double *kk = NULL, *aa;
    static int	  size;

    if(kk == NULL){
	kk = dgetmem(m+m+2);
	aa = kk + m + 1;
	size = m;
    }

    if(m > size){
	free(kk);
	kk = dgetmem(m+m+2);
	aa = kk + m + 1;
	size = m;
    }

    movem(a, aa, sizeof(*aa), m+1);

    kk[0] = aa[0];
    for (n=m; n>=1; n--){
	movem(&aa[1], &kk[1], sizeof(*aa), n);

	if (kk[n] >= 1.0 || kk[n] <= -1.0) flg = -1;

	s = 1.0 - kk[n] * kk[n];
	for (i=1; i<n; i++)
	    aa[i] = (kk[i] - kk[n] * kk[n-i]) / s;
    }
    movem(kk, k, sizeof(*kk), m+1);
    return(flg);
}
