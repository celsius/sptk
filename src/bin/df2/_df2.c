/******************************************************
 $Id$
  2nd order standard form digital filter
	double df2(x,sf,f0p,wbp,f0z,wbz,fp,fz,buf,bufp)

	double	x     : filter input
	double	sf    : sampling frequency
	double	f0p   : center frequency of pole
	double	wbp   : band width of pole
	double    f0z   : center frequency of zero
	double    wbz   : band witdth of zero
	int	fp    : if pole is exist then fp = 1 
	int	fz    : if zero is exist then fz = 1
	double	buf[] : buffer ( which require 3 )
	int	*bufp : work pointer
	
	return	      : filter output

		Dec.1995 Naohiro Isshiki
*******************************************************/	 
#include<math.h>

double df2(x,sf,f0p,wbp,f0z,wbz,fp,fz,buf,bufp)
double 	x,sf,f0p,wbp,f0z,wbz;
int	fp,fz;
double	buf[];
int	*bufp;


{
	double a[3], b[3];
	double p, e;
	double dfs();
	
	p = 4 * atan(1.0) / sf;
	e = exp(-p * wbz);

	a[0] = 1.0;
	if(fz){
		a[1] = -2 * e * cos(2 * p * f0z);
		a[2] = e * e;
	}else{
		a[1] = 0;
		a[2] = 0;
	}
		
	e = exp(-p * wbp);
	b[0] = 1.0;
	if(fp){
		b[1] = -2 * e * cos(2 * p * f0p);
		b[2] = e * e;
	} else {
		b[1] = 0;
		b[2] = 0;
	}
	return(dfs(x,b,2,a,2,buf,bufp));
	
}


