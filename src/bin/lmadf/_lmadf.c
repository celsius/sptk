/****************************************************************

    $Id$

    LMA Digital Filter

	double	lmadflt(x, c, m, pd, d)

	double	x   : input
	double	*c  : cepstrum
	int	m   : order of cepstrum
	int	pd  : order of pade approximation
	double  *d  : delay

	return value : filtered data

*****************************************************************/

#include <stdio.h>
#include <SPTK.h>


static double pade[] = {1.0,
			1.0, 0.0,
			1.0, 0.0,	0.0,
			1.0, 0.0,	0.0,	   0.0,
			1.0, 0.4999273, 0.1067005, 0.01170221, 0.0005656279,
			1.0, 0.4999391, 0.1107098, 0.01369984, 0.0009564853, 0.00003041721};

double *ppade;

double lmadf(x, c, m, pd, d)
double x, *c, *d;
int m, pd;
{
    double lmadf1();

    ppade = &pade[pd*(pd+1) / 2];

    x = lmadf1(x, c, m, d, 1, 1, pd);			/* D1(z) */
    x = lmadf1(x, c, m, &d[(m+1)*pd], 2, m, pd);	/* D2(z) */

    return(x);
}

/****************************************************************

	double	lmadf1(x, c, m, d, m1, m2, pd)

	double	x  : input
	double	*c : cepstrum
	int	m  : order of cepstrum
	double  *d : delay
	int	m1 : start order
	int	m2 : end order
	int	pd : order of pade approximation

*****************************************************************/

double lmadf1(x, c, m, d, m1, m2, pd)
double x, *c, *d;
int m, m1, m2, pd;
{
    double 	y, lmafir(), t, *pt;
    int 	i;
	
    pt = &d[pd*m];
    t = lmafir(pt[pd-1], c, m, &d[(pd-1)*m], m1, m2);
    y = (t *= ppade[pd]);
    x += (1 & pd) ? t : -t;
    for (i=pd-1; i>=1; i--){
	pt[i] = t = lmafir(pt[i-1], c, m, &d[(i-1)*m], m1, m2);
	y += (t *= ppade[i]);
	x += (1 & i) ? t : -t;
    }
    y += (pt[0] = x);

    return(y);
}

/****************************************************************

	double	lmafir(x, c, d, m, m1, m2)

	double	x  : input
	double	*c : cepstrum
	int	m  : order of cepstrum
	double  *d : delay
	int	m1 : start order
	int	m2 : end order

*****************************************************************/

double lmafir(x, c, m, d, m1, m2)
double x, *c, *d;
int m, m1, m2;
{
    int i;

    for (i=m-1; i>=1; i--) d[i] = d[i-1];
    d[0] = x;
    for (x=0.0,i=m1; i<=m2; i++) x += c[i] * d[i-1];

    return(x);
}

