/****************************************************************
    $Id: glog.c,v 1.1.1.1 2000/03/01 13:58:55 yossie Exp $

    Generalized Logarithmic Function (real argument)

	double glog(r, x)

	double r : gamma
	double x : argument

*****************************************************************/

double glog(r, x)
double r, x;
{
    double log(), pow();

    if (r == 0.0)
	return(log(x));
    else
	if (r < 0.)
	    return((pow(1 / x, -r) - 1.0) / r);
	else
	    return((pow(x, r) - 1.0) / r);
}
