int	histogram(x, size, min, max, step, h)
double	*x;
double	min, max, step;
int	size, *h;
{
	int	k, ii, flg=0;
	register int	jj;

	k = (int)((max - min) / step + 1.0);

	fillz(h, sizeof(*h), k);

	for (ii=0; ii<size; ii++) {
		if (x[ii] < min || x[ii] > max) flg =1;
		else for (jj=0; jj<k; jj++)
			if (x[ii] < min + (jj+1) * step) {
				h[jj]++;
				break;
			}
	}
	return flg;
}
