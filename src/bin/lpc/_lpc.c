/****************************************************************

    $Id: _lpc.c,v 1.1.1.1 2000/03/01 13:58:39 yossie Exp $

    LPC Analysis Using Levinson-Durbin method

   	int	lpc(x, flng, a, m);

	double  *x   : input sequence
	int    	flng : flame length
	double  *a   : LP coefficients
	int    	m    : order of LPC

	return value : 	0  -> normally completed
			-1 -> abnormally completed
			-2 -> unstable LPC

******************************************************************/

#include <stdio.h>
#include <SPTK.h>

int lpc(x, flng, a, m)
double *x, *a;
int flng, m;
{
    int		  flag;
    static double *r = NULL;
    static int	  size;
    
    if(r == NULL){
	r = dgetmem(m+1);
	size = m;
    }
    if(m > size){
	free(r);
	r = dgetmem(m+1);
	size = m;
    }
    
    acorr(x, flng, r, m);
    flag = levdur(r, a, m, -1);

    return(flag);
}
