/***************************************************************
    $Id$

    Magnitude Squared Generalized Exponential Function	

	double agexp(r, x, y)

	double	r : gamma
	double  x : real part
	double  y : imaginary part

*****************************************************************/

#include <stdio.h>

double agexp(r, x, y)
double r, x, y;
{
    double exp(), pow(), w;

    if (r == 0.0)
	return(exp(2 * x));
    else{
	x = 1 + r * x;
	y = r * y;
	w = x * x + y * y;
	if (r < 0.)
	    return(pow(1/w, -1/r));
	else
	    return(pow(w, 1/r));
    }
}
