/****************************************************************

    $Id: _mglsadf.c,v 1.1.1.1 2000/03/01 13:58:44 yossie Exp $

    MGLSA Digital Filter

	double	mglsadf(x, b, m, a, n, d)

	double	x   : input
	double	*b  : filter coefficients (K, g*b'(1), ..., g*b'(m))
	int	m   : order of cepstrum
	double  a   : alpha
	int     n   : -1/gamma
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double mglsadf(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  mglsadff();
    
    for(i=0; i<n; i++)
	x = mglsadff(x, b, m, a, &d[i*(m+1)]);
    
    return(x);
}

double mglsadff(x, b, m, a, d)
double	x, *b, a, *d;
int	m;
{
    register int	i;
    double		y, aa;

    aa = 1 - a * a;

    y = d[0] * b[1];
    for(i=1; i<m; i++){
	d[i] += a * (d[i+1] - d[i-1]);
	y += d[i] * b[i+1];
    }
    x -= y;
    
    for(i=m; i>0; i--)
	d[i] = d[i-1];
    d[0] = a * d[0] + aa * x;

    return (x);
}

double mglsadf1(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  g, mglsadff1();

    g = -1.0 / (double)n;
    
    for(i=0; i<n; i++)
	x = mglsadff1(x, b, m, a, g, &d[i*(m+1)]);
    
    return(x);
}

double mglsadff1(x, b, m, a, g, d)
double	x, *b, a, g, *d;
int	m;
{
    register int	i;
    double		y, aa;

    aa = 1 - a * a;

    y = d[0] * b[1];
    for(i=1; i<m; i++){
	d[i] += a * (d[i+1] - d[i-1]);
	y += d[i] * b[i+1];
    }
    x -= g * y;
    
    for(i=m; i>0; i--)
	d[i] = d[i-1];
    d[0] = a * d[0] + aa * x;

    return (x);
}

double mglsadft(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  mglsadfft();
    
    for(i=0; i<n; i++)
	x = mglsadfft(x, b, m, a, &d[i*(m+1)]);
    
    return(x);
}

double mglsadfft(x, b, m, a, d)
double	x, *b, a, *d;
int	m;
{
    register int	i;
    
    x -= d[0] * (1.0 - a * a);
    
    d[m] = b[m] * x + a * d[m-1];
    for(i=m-1; i>=1; i--)
	d[i] += b[i] * x + a * (d[i-1] - d[i+1]);

    for(i=0; i<m; i++)
	d[i] = d[i+1];
    
    return (x);
}

double mglsadf1t(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  g, mglsadff1t();

    g = -1.0 / (double)n;
    
    for(i=0; i<n; i++)
	x = mglsadff1t(x, b, m, a, g, &d[i*(m+1)]);
    
    return(x);
}

double mglsadff1t(x, b, m, a, g, d)
double	x, *b, a, g, *d;
int	m;
{
    register int	i;
    double		y, aa;

    x -= d[0] * (1.0 - a * a) * g;
    
    d[m] = b[m] * x + a * d[m-1];
    for(i=m-1; i>=1; i--)
	d[i] += b[i] * x + a * (d[i-1] - d[i+1]);

    for(i=0; i<m; i++)
	d[i] = d[i+1];

    return (x);
}
