/***************************************************************
    $Id$

    Normalize coefficients

	void	norm0(x, y, m)

	double	*x  : input coefficients
	double	*y  : normalized coefficients
	int	m   : order of coefficients

***************************************************************/

void norm0(x, y, m)
double *x, *y;
int m;
{
    y[0] = 1 / x[0];
    for(; m>=1; m--)
	y[m] = x[m] * y[0];
}
