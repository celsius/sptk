/***************************************************************
    $Id: _rmse.c,v 1.1.1.1 2000/03/01 13:58:50 yossie Exp $

    Calculation of Root Mean Squared Error

	double	rmse(x, y, n)

	double	*x  : data sequence
	double	*y  : data sequence
	int	n   : data length

	return value :  RMSE of data

***************************************************************/

double rmse(x, y, n)
double *x, *y;
int n;
{
    register int  i;
    double	  sub, sum, sqrt();

    sum = 0.0;
    for(i=0; i<n; i++) {
	sub = x[i] - y[i];
	sum += sub * sub;
    }

    return( sqrt(sum / n) );
}
