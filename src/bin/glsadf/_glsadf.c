/****************************************************************

    $Id: _glsadf.c,v 1.1.1.1 2000/03/01 13:58:35 yossie Exp $

    GLSA Digital Filter

	double	glsadf(x, c, m, n, d)

	double	x   : input
	double	*c  : filter coefficients (K, g*c'(1), ..., g*c'(m))
	int	m   : order of cepstrum
	int     n   : -1/gamma
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double glsadf(x, c, m, n, d)
double x, *c, *d;
int m, n;
{
    int  	i;
    double      poledf();
    
    for(i=0; i<n; i++)
	x = poledf(x, c, m, &d[m*i]);

    return(x);
}

double glsadf1(x, c, m, n, d)
double x, *c, *d;
int m, n;
{
    int  	i;
    double      gamma, gpoledf();

    gamma = -1 / (double)n;
    
    for(i=0; i<n; i++)
	x = gpoledf(x, c, m, gamma, &d[m*i]);
    
    return(x);
}

double gpoledf(x, c, m, g, d)
double x, *c, *d, g;
int m;
{
    double   y = 0.0;
    
    for(m--; m>0; m--){
	y -= c[m+1] * d[m];
	d[m] = d[m-1];
    }
    y -= c[1] * d[0];
    y *= g;
    d[0] = (x += y);
    
    return(x);
}

