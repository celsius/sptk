/****************************************************************

    $Id: _lspdf.c,v 1.1 2000/03/01 13:58:41 yossie Exp $

    LSP Speech Synthesis Digital Filter

	double	lspdf_even(x, a, m, d)

	double	x   : input
	double	*f  : LSP coefficients
	int	m   : order of coefficients
	double  *d  : delay

	return value : filtered data

*****************************************************************/
#include <stdio.h>

double	lspdf_even(x, f, m, d)
double	x, *f, *d;
int	m;
{
    double  	  *d1, *d2, *lsp, x1, x2, cos();
    register int  i;

    d1 = d + 1;
    d2 = d1 + m;
    lsp = f + 1;
    x1 = x2 = d[0];

    for(i=0; i<m; i+=2){
	d1[i] -= 2.0 * x1 * cos(lsp[i]);
	d2[i] -= 2.0 * x2 * cos(lsp[i+1]);
	d1[i+1] += x1;
	d2[i+1] += x2;
	x += d1[i] + d2[i];
	x1 = d1[i+1];
	x2 = d2[i+1];
    }
    
    x -= d2[m-1] - d1[m-1];

    for(i=m-1; i>0; i--){
	d1[i] = d1[i-1];
	d2[i] = d2[i-1];
    }
    d1[0] = d2[0] = d[0];
    d[0] = -0.5 * x;

    return(x);
}

double	lspdf_odd(x, f, m, d)
double	x, *f, *d;
int	m;
{
    register int  i;
    int           mh1, mh2;
    double  	  *d1, *d2, *lsp, x1, x2, cos();

    mh1 = (m+1) / 2;
    mh2 = (m-1) / 2;

    d1 = d + 1;
    d2 = d1 + (mh1+mh1-1);
    lsp = f + 1;
    x1 = x2 = d[0];

    for(i=0; i<m-1; i+=2){
	d1[i] -= 2.0 * x1 * cos(lsp[i]);
	d2[i] -= 2.0 * x2 * cos(lsp[i+1]);
	d1[i+1] += x1;
	d2[i+1] += x2;
	x += d1[i] + d2[i];
	x1 = d1[i+1];
	x2 = d2[i+1];
    }
    d1[i] -= 2.0 * x1 * cos(lsp[i]);
    x += d1[i] - d2[i];

    for(i=m-1; i>0; i--){
	d1[i] = d1[i-1];
	d2[i] = d2[i-1];
    }
    d1[0] = d2[0] = d[0];
    d[0] = -0.5 * x;

    return(x);
}

