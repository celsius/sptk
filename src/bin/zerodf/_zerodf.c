/****************************************************************

    $Id$

    All Zero Digital Filter

	double	zerodf(x, b, m, d)

	double	x   : input
	double	*b  : MA coefficients
	int	m   : order of coefficients
	double  *d  : delay

	return value : filtered data

*****************************************************************/

double	zerodf(x, b, m, d)
double	x, *b, *d;
int	m;
{
    register int  i;
    double 	  out;

    out = b[0]*x;

    for(m--; m>0; m--){
	out += b[m+1] * d[m];
	d[m] = d[m-1];
    }
    out += b[1] * d[0];
    d[0] = x;
    
    return(out);
}

double	zerodft(x, b, m, d)
double	x, *b, *d;
int	m;
{
    register int        i;
    double              out;

    out = b[0]*x + d[0];

    for(i=1; i<m; i++)
	d[i-1] = b[i] * x + d[i];

    d[m-1] = b[m] * x;
    
    return(out);
}

double	zerodf1(x, b, m, d)
double	x, *b, *d;
int	m;
{
    register int  i;
    double 	  out;

    out = x;
    for(m--; m>0; m--){
	out += b[m+1] * d[m];
	d[m] = d[m-1];
    }
    out += b[1] * d[0];
    d[0] = x;
    
    return(out);
}

double	zerodf1t(x, b, m, d)
double	x, *b, *d;
int	m;
{
    register int        i;
    double              out;

    out = x + d[0];

    for(i=1; i<m; i++)
	d[i-1] = b[i] * x + d[i];

    d[m-1] = b[m] * x;
    
    return(out);
}
