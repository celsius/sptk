/****************************************************************

    $Id: _ltcdf.c,v 1.1.1.1 2000/03/01 13:58:41 yossie Exp $

    All-Pole Lattice Digital Filter

	double	ltcdf(x, k, m, d)

	double	x   : input
	double	*k  : reflection coefficients
	int	m   : order of coefficients
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double ltcdf(x, k, m, d)
double x, *k, *d;
int m;
{
    x -= k[m] * d[m-1];
    for (m--; m>=1; m--){
	x -= k[m] * d[m-1];
	d[m] = d[m-1] + k[m] * x;
    }
    d[0] = x;

    return(x);
}

