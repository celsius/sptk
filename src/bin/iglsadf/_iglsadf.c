/****************************************************************

    $Id$

    Inverse GLSA Digital Filter

	double	iglsadf(x, c, m, n, d)

	double	x   : input
	double	*c  : filter coefficients (K, g*c'(1), ..., g*c'(m))
	int	m   : order of cepstrum
	int     n   : -1/gamma
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double iglsadf(x, c, m, n, d)
double x, *c, *d;
int m, n;
{
    int  	i;
    double      zerodf1();
    
    for(i=0; i<n; i++)
	x = zerodf1(x, c, m, &d[m*i]);

    return(x);
}

double iglsadf1(x, c, m, n, d)
double x, *c, *d;
int m, n;
{
    int  	i;
    double      gamma, gzerodf();

    gamma = -1 / (double)n;
    
    for(i=0; i<n; i++)
	x = gzerodf(x, c, m, gamma, &d[m*i]);
    
    return(x);
}

double gzerodf(x, c, m, g, d)
double x, *c, *d, g;
int m;
{
    double   y = 0.0;
    
    for(m--; m>0; m--){
	y += c[m+1] * d[m];
	d[m] = d[m-1];
    }
    y += c[1] * d[0];
    d[0] = x;

    x += y * g;

    return(x);
}

