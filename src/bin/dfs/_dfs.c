/************************************************************************
	$Id: _dfs.c,v 1.1.1.1 2000/03/01 13:58:31 yossie Exp $								
	Standard Form Digital Filter					
	latice digital filter						
		double dfs(x, a, m, b, n, buf, bufp)			
									
		double x             : filter input			
		double a[0]          : gain
		double a[1],...,a[m] : AR coefficints
		int  m   	     : order of AR part
		double b[0],...,b[n] : MA coefficents
		int  n		     : order of MA part
		double buf[]	     : buffer
		int *bufp	     : pointer for buffer
		return		     : filter output	
		1987.11  K.Tokuda					
		1995.12  N.Isshiki					
************************************************************************/
#include <stdio.h>


double dfs(x, a, m, b, n, buf, bufp)
	double x;
	double a[];
	double b[];
	int m;
	int n;
	double buf[];
	int *bufp;
{
	double y = 0.0;
	int i, p;
	int max;

	n++;
	m++;

	(m < n)? (max = n) : (max = m);

	x = x * a[0];
	for (i = 1; i < m; i++){
		if ((p = *bufp + i) >= max)
			p -= max;
		x -= buf[p] * a[i];
	}
	buf[*bufp] = x;
	for (i = 0; i < n; i++){
		if ((p = *bufp + i) >= max)
			p -= max;
		y += buf[p] * b[i];
	}

	if (--*bufp < 0)
		*bufp += max;

	return(y);

}
