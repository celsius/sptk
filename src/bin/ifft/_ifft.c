/************************************************
* NAME:						*
*	ifft - Inverse Fast Fourier Transform	*   
* SYNOPSIS:					*
*	int	ifft(x, y, m)			*
*						*
*	real	x[];	real part 		*
*	real	y[];	imaginary part		*
*	int	m;	size of FFT		*
************************************************/


ifft(x, y, m)
register double	*x, *y;
int		m;
{
	register int	i;

	if(fft(y, x, m) == -1)
		return(-1);

	for(i = m; --i >= 0; ++x, ++y) {
		*x /= m; 
		*y /= m;
	}

	return(0);
}
