/****************************************************************

    $Id$

    MLSA Digital Filter

	double	mlsadf(x, b, m, a, pd, d)

	double	x   : input
	double	*c  : MLSA filter coefficients
	int	m   : order of cepstrum
	double  a   : all-pass constant
	int	pd  : order of pade approximation
	double  *d  : delay

	return value : filtered data

*****************************************************************/

#include <stdio.h>

static double pade[] = {1.0,
			1.0, 0.0,
			1.0, 0.0,       0.0,
			1.0, 0.0,       0.0,       0.0,
			1.0, 0.4999273, 0.1067005, 0.01170221, 0.0005656279,
			1.0, 0.4999391, 0.1107098, 0.01369984, 0.0009564853, 0.00003041721 };

double *ppade;

double mlsadf(x, b, m, a, pd, d)
double x, *b, *d, a;
int m, pd;
{
    double mlsadf1(), mlsadf2();

    ppade = &pade[pd*(pd+1) / 2];
    
    x = mlsadf1(x, b, m, a, pd, d);
    x = mlsadf2(x, b, m, a, pd, &d[2*(pd+1)]);

    return(x);
}

double mlsadf1(x, b, m, a, pd, d)
double x, *b, *d, a;
int m, pd;
{
    double	 v, out = 0.0, *pt, aa;
    register int i;
	
    aa = 1 - a*a;
    pt = &d[pd+1];

    for(i=pd; i>=1; i--){
	d[i] = aa*pt[i-1] + a*d[i];
	pt[i] = d[i] * b[1];
	v = pt[i] * ppade[i];
		
	x += (1 & i) ? v : -v;
	out += v;
    }
	
    pt[0] = x;
    out += x;
	
    return(out);
}

double mlsadf2(x, b, m, a, pd, d)
double x, *b, *d, a;
int m, pd;
{
    double	 v, out = 0.0, *pt, aa, mlsafir();
    register int i;
    
    aa = 1 - a*a;
    pt = &d[pd * (m+2)];
	
    for(i=pd; i>=1; i--){
	pt[i] = mlsafir(pt[i-1], b, m, a, &d[(i-1)*(m+2)]);
	v = pt[i] * ppade[i];

	x  += (1&i) ? v : -v;
	out += v;
    }
    
    pt[0] = x;
    out  += x;
	
    return(out);
}

double mlsafir(x, b, m, a, d)
double x, *b, *d, a;
int m;
{
    double	 y = 0.0, aa;
    register int i;
	
    aa = 1 - a*a;

    d[0] = x;
    d[1] = aa*d[0] + a*d[1];
	
    for(i=2; i<=m; i++){
	d[i] = d[i] + a*(d[i+1]-d[i-1]);
	y += d[i]*b[i];
    }
	
    for(i=m+1; i>1; i--) d[i] = d[i-1];
	
    return (y);
}

