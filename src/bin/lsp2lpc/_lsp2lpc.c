/****************************************************************

    $Id: _lsp2lpc.c,v 1.1.1.1 2000/03/01 13:58:40 yossie Exp $

    Transformation LSP to LPC

	void lsp2lpc(lsp, a, m)

	double  *lsp : LSP
	double  *a   : LPC
	int    	m    : order of LPC

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>

void lsp2lpc(lsp, a, m)
double *lsp, *a;
int m;
{
    int		  i, k, mh1, mh2, flag_odd;
    double	  xx, xf, xff, cos();
    static double *f = NULL, *p, *q, *a0, *a1, *a2, *b0, *b1, *b2;
    static int	  size;

    flag_odd = 0;
    if(m % 2 == 0)
        mh1 = mh2 = m / 2;
    else{
        mh1 = (m + 1) / 2;
	mh2 = (m - 1) / 2;
	flag_odd = 1;
    }

    if(f == NULL){
	f = dgetmem(5*m+6);
	p  = f  + m;       q  = p  + mh1;
	a0 = q  + mh2;     a1 = a0 + (mh1+1);
	a2 = a1 + (mh1+1); b0 = a2 + (mh1+1);
	b1 = b0 + (mh2+1); b2 = b1 + (mh2+1);
	size = m;
    }
    if(m > size){
	free(f);
	f = dgetmem(5*m+6);
	p  = f  + m;       q  = p  + mh1;
	a0 = q  + mh2;     a1 = a0 + (mh1+1);
	a2 = a1 + (mh1+1); b0 = a2 + (mh1+1);
	b1 = b0 + (mh2+1); b2 = b1 + (mh2+1);
	size = m;
    }
    
    movem(lsp, f, sizeof(*lsp), m);

    fillz(a0, sizeof(*a0), mh1+1); fillz(b0, sizeof(*b0), mh2+1);
    fillz(a1, sizeof(*a1), mh1+1); fillz(b1, sizeof(*b1), mh2+1);
    fillz(a2, sizeof(*a2), mh1+1); fillz(b2, sizeof(*b2), mh2+1);

    /* lsp filter parameters */
    for (i=k=0; i<mh1; i++,k+=2)
	p[i] = -2.0 * cos(PI2 * f[k]);
    for (i=k=0; i<mh2; i++,k+=2)
	q[i] = -2.0 * cos(PI2 * f[k+1]);

    /* impulse response of analysis filter */
    xx = 1.0;
    xf = xff = 0.0;
    for (k=0; k<=m; k++){
        if(flag_odd){
	    a0[0] = xx;
	    b0[0] = xx - xff;
	    xff = xf;
	    xf  = xx;
	}
	else{
	    a0[0] = xx + xf;
	    b0[0] = xx - xf;
	    xf = xx;
	}

	for (i=0; i<mh1; i++){
	    a0[i+1] = a0[i] + p[i] * a1[i] + a2[i];
	    a2[i] = a1[i]; a1[i] = a0[i];
	}
	for (i=0; i<mh2; i++){
	    b0[i+1] = b0[i] + q[i] * b1[i] + b2[i];
	    b2[i] = b1[i]; b1[i] = b0[i];
	}

	if (k != 0)
	    a[k-1] = -0.5 * (a0[mh1] + b0[mh2]);

	xx = 0.0;
    }

    for (i=m-1; i>=0; i--)
	a[i+1] = -a[i];
    a[0] = 1.0;
}
