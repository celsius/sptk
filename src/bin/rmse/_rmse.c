/***************************************************************
    $Id$

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
