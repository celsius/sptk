/***********************************************************
 $Id: _zcross.c,v 1.1.1.1 2000/03/01 13:58:55 yossie Exp $
	obtain zero coss
		zcross(x, fl, n)

		double 	*x	: input sequence
		int	fl 	: frame length
		double	n	: flag of normalize

		Naohiro Isshiki, 	March 1996	
***********************************************************/
#include<math.h>

double sgn(x)
double x;
{
	if (x >=0)
		return(0.5);
	else 
		return(-0.5);

}

double zcross(x, fl, n)
double	*x;
int	fl;
int	n;
{
	int	i;
	double	z = 0;

	for (i = 0; i < fl; i++) 
		z += abs(sgn(x[i+1]) - sgn(x[i]));		
	if (n)
		z /= fl;
	return(z);
}
