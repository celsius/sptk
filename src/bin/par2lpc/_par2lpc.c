/***************************************************************

    $Id: _par2lpc.c,v 1.1 2000/03/01 13:58:48 yossie Exp $

    Transformation PARCOR to LPC

	void par2lpc(k, a, m)

	double  *k   : PARCOR coefficients
	double  *a   : LP coefficients	
	int     m    : order of LPC

****************************************************************/

void par2lpc(k, a, m)
double *k, *a;
int  m;
{
    int 	i, n, flg = 0;

    a[0] = k[0];
    for (n=1; n<=m; n++){
	for (i=1; i<n; i++)
	    a[i] = k[i] + k[n] * k[n-i];
	movem(&a[1], &k[1], sizeof(*a), n-1);
    }
    a[m] = k[m];
}

