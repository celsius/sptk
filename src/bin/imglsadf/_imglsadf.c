/****************************************************************

    $Id: _imglsadf.c,v 1.1.1.1 2000/03/01 13:58:37 yossie Exp $

    Inverse MGLSA Digital Filter

	double	imglsadf(x, b, m, a, n, d)

	double	x   : input
	double	*b  : filter coefficients (K, g*b'(1), ..., g*b'(m))
	int	m   : order of cepstrum
	double  a   : alpha
	int     n   : -1/gamma
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double imglsadf(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  imglsadff();
    
    for(i=0; i<n; i++)
	x = imglsadff(x, b, m, a, &d[i*(m+1)]);
    
    return(x);
}

double imglsadff(x, b, m, a, d)
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
    y += x;
    
    for(i=m; i>0; i--)
	d[i] = d[i-1];
    d[0] = a * d[0] + aa * x;

    return (y);
}

double imglsadf1(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  g, imglsadff1();

    g = -1.0 / (double)n;
    
    for(i=0; i<n; i++)
	x = imglsadff1(x, b, m, a, g, &d[i*(m+1)]);
    
    return(x);
}

double imglsadff1(x, b, m, a, g, d)
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
    y = g * y + x;
    
    for(i=m; i>0; i--)
	d[i] = d[i-1];
    d[0] = a * d[0] + aa * x;

    return (y);
}

double imglsadft(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  imglsadfft();
    
    for(i=0; i<n; i++)
	x = imglsadfft(x, b, m, a, &d[i*(m+1)]);
    
    return(x);
}

double imglsadfft(x, b, m, a, d)
double	x, *b, a, *d;
int	m;
{
    register int	i;
    double		y;

    y = x + (1.0 - a * a) * d[0];
    
    d[m] = b[m] * x + a * d[m-1];
    for(i=m-1; i>=1; i--)
	d[i] += b[i] * x + a * (d[i-1] - d[i+1]);
    
    for(i=0; i<m; i++)
	d[i] = d[i+1];

    return (y);
}

double imglsadf1t(x, b, m, a, n, d)
double x, *b, a, *d;
int m, n;
{
    int     i;
    double  g, imglsadff1t();

    g = -1.0 / (double)n;
    
    for(i=0; i<n; i++)
	x = imglsadff1t(x, b, m, a, g, &d[i*(m+1)]);
    
    return(x);
}

double imglsadff1t(x, b, m, a, g, d)
double	x, *b, a, g, *d;
int	m;
{
    register int	i;
    double		y;

    y = x + g * (1.0 - a * a) * d[0];
    
    d[m] = b[m] * x + a * d[m-1];
    for(i=m-1; i>=1; i--)
	d[i] += b[i] * x + a * (d[i-1] - d[i+1]);
    
    for(i=0; i<m; i++)
	d[i] = d[i+1];

    return (y);
}

