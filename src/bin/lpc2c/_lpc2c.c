/****************************************************************

    $Id$

    Transformation LPC to Cepstrum 

	void 	lpc2c(a, m1, c, m2)

	double  *a : LP coefficients
	int   	m1 : order of LPC
	double  *c : cepstral coefficients
	int   	m2 : order of cepstrum

****************************************************************/

void lpc2c(a, m1, c, m2)
double *a, *c;
int m1, m2;
{
    register int 	i, k, upl;
    double	 	log(), exp(), d;

    c[0] = log(a[0]);
    c[1] = - a[1];
    for(k = 2; k <= m2; ++k){
	upl = (k > m2) ? m2+1 : k;
	
	for(d=0.0, i=(k>m1) ? k-m1 : 1; i<upl; i++)
	    d += i * c[i] * a[k-i];
	c[k] = -d / k;
	
	if(k <= m1) c[k] -= a[k];
    }
}

