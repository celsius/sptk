/************************************************
*	Data clipping				*
*		1996.3.14			*
*						*
*	synopsis:				*
*		clip(x, l, min, max, y)		*
*						*
*		double *x : input sequence	*
*		int     l : sequence length	*
*		double min: lower bound		*
*		double max: upper bound		*
*		double *y : output area		*
*						*
************************************************/

void clip(x, l, min, max, y)
double	*x, *y, min, max;
int	l;
{
	register int	i;
	for (i=0; i<l; i++)
		y[i] = (x[i] < min)?min:(x[i] > max)?max:x[i];
}
