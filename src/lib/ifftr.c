/***************************************************************
    $Id$

    Inverse Fast Fourier Transform for Real Sequence

	int	ifftr(x, y, l)

	double	*x  : real part of data
	double	*y  : working area
	int	l   : number of data(radix 2)

***************************************************************/

int ifftr(x, y, l)
double *x, *y;
int l;
{
    register int i;
    register double *xp, *yp;

    fftr(x, y, l);

    xp = x; yp = y; 
    i = l;
    while (i--){
	*xp++ /= l;
	*yp++ /= -l;
    }
}
