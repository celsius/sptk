/******************************************************************
 $Id: _c2ir.c,v 1.1 2000/03/01 13:58:30 yossie Exp $

	cepstrum to impulse response
		c2ir(c,nc,h,leng)

		double	*c   : cepstral sequence
		int	nc   : order of cepstrum
		double	*h   : impulse response
		int	leng : length of impulse response

	impulse response to cepstrum
		ic2ir(h,leng,nc,c)

		double	*h   : impulse response
		int	nc   : order of cepstrum
		double	*c   : cepstral sequence
		int	leng : length of impulse response

			Naohiro Isshiki,	Dec.1995
********************************************************************/

void c2ir(c,nc,h,leng)
double 	*c,*h;
int 	leng,nc;
{
	register int	n, k, nr, upl;
	double	d, exp(), log();

	h[0] = exp(c[0]);
	for(n = 1; n < leng; ++n) {
		d = 0;
		upl = (n >= nc) ? nc - 1 : n;
		for(k = 1; k <= upl; ++k)
			d += k * c[k] * h[n - k];
		h[n] = d / n;
	}
}

void ic2ir(h,leng,c,nc)
double 	*h,*c;
int 	leng,nc;
{
	register int	n, k, nr, upl;
	double	d, exp(), log();

	c[0] = log(h[0]);
	for(n = 1; n < nc; ++n) {
		d = (n >= leng) ? 0 : n * h[n];
		upl = (n > leng) ? n - leng + 1 : 1;
		for(k = upl; k < n; ++k)
			d -= k * c[k] * h[n - k];
		c[n] = d / (n * h[0]);
	}
}
 
