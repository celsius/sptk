/****************************************************************

    $Id: _poledf.c,v 1.1.1.1 2000/03/01 13:58:35 yossie Exp $

    All Pole Digital Filter

	double	poledf(x, a, m, d)

	double	x   : input
	double	*a  : AR coefficients
	int	m   : order of coefficients
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double	poledf(x, a, m, d)
double	x, *a, *d;
int	m;
{
    for(m--; m>0; m--){
	x -= a[m+1] * d[m];
	d[m] = d[m-1];
    }
    x -= a[1] * d[0];
    d[0] = x;
    
    return(x);
}

double poledft(x, a, m, d)
double x, *a, *d;
int    m;
{
    register int  i;
    
    x -= d[0];
    for(i=1; i<m; i++)
	d[i-1] = d[i] + a[i] * x;
    d[m-1] = a[m] * x;    

    return(x);
}

