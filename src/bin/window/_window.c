/****************************************************************
$Id: _window.c,v 1.1 2000/03/01 13:58:52 yossie Exp $

			Window function
			---------------

	double  window( name, x, size, pnflg );

		char	*name;	window name

			blackman, hamming,
			hanning,  bartlett, trapezoid

		real	*x;	1 frame data
		int	size;	window(frame) size
		int	nflg;	normalizing flag

			nflg = 0 : don't normalize
			       1 : normalize by power
			       2 : normalize by magnitude

    set windowed value to "*x" and return "normalizing gain".
*****************************************************************/
#include	<stdio.h>
#include	<SPTK.h>

#define	M_2PI   (2 * 3.14159265358979323846) 

double	cos();
double	sqrt();
double	*dgetmem();

double*	blackman();
double*	hamming();
double*	hanning();
double*	bartlett();
double*	trapezoid();
double*	rectangular();


double window( type, x, size, nflg )
	Window	type;
	double	*x;
	int	size, nflg;
{
	register int	i;
	static double	g;
	static double	*w = NULL;
	static Window	ptype = -1;
	static int	psize = -1, pnflg = -1;

	if ((type != ptype) || (size != psize) || (nflg != pnflg)) {
	    if (size > psize) {
		if (w != NULL)
		    free(w);
		w = dgetmem(size);
	    }

	    switch (type)  {
		case BLACKMAN:     blackman(w, size);		break;
		case HAMMING:      hamming(w, size);		break;
		case HANNING:      hanning(w, size);		break;
		case BARTLETT:     bartlett(w, size);		break;
		case TRAPEZOID:    trapezoid(w, size);		break;
		case RECTANGULAR:  rectangular(w, size);	break;
		default:
		    fprintf(stderr, "window : unknown window type\n");
		    exit(1);
	    }

	    switch (nflg) {
		case 1:
		    for (i = 0, g = 0.0; i < size; i++)
			g += w[i] * w[i];
		    g = sqrt(g);
		    for (i = 0; i < size; i++)
			w[i] /= g;
		    break;
		case 2:
		    for (i = 0, g = 0.0; i < size; i++)
			g += w[i];
		    for (i = 0; i < size; i++)
			w[i] /= g;
		    break;
		case 0:
		default:
		    g = 1.0;
	    }

	    ptype = type;
	    psize = size;
	    pnflg = nflg;
	}

	for (i = 0; i < size; i++)
		x[i] = x[i] * w[i];

	return (g);
}


/************************************************
		Blackman window

	double  *blackman(w, leng)

	double	*w;	window values
	int	leng;	window length
************************************************/

double  *blackman(w, leng)
	double	*w;
	int	leng;
{
	register int	i;
	double		arg, x;
	register double	*p;

	arg = M_2PI / (leng - 1);
	for (p=w, i=0; i<leng; i++)  {
		x = arg * i;
		*p++ = 0.42 - 0.50 * cos(x) + 0.08 * cos(x+x);
	}
	return( w );
}


/************************************************
		Hamming window

	double  *hamming(w, leng)
	double	*w;	window values
	int	leng;	window length
************************************************/

double  *hamming(w, leng)
	double	*w;
	int	leng;
{
	register int	i;
	double		arg;
	register double	*p;

	arg = M_2PI / (leng - 1);
	for (p=w, i=0; i<leng; i++)
		*p++ = 0.54 - 0.46 * cos(i * arg);

	return ( w );
}


/************************************************
		Hanning window

	double  *hanning(w, leng)
	double	*w;	window values
	int	leng;	window length
************************************************/

double  *hanning(w, leng)
	double	*w;
	int	leng;
{
	register int	i;
	double		arg;
	register double	*p;

	arg = M_2PI / (leng - 1);
	for (p=w, i=0; i<leng; i++)
		*p++ = 0.5 * (1 - cos(i * arg));

	return ( w );
}


/************************************************
		Bartlett window

	double  *bartlett(w, leng)

	double	*w;	window values
	int	leng;	window length
************************************************/

double  *bartlett(w, leng)
	double	*w;
	int	leng;
{
	register int	k, m;
	register double	*p, slope;

	m = leng / 2;
	slope = 2.0 / (double)(leng - 1);

	for (k = 0, p = w; k < m; k++)
		*p++ = slope * k;
	for ( ; k < leng; k++)
		*p++ = 2.0 - slope * k;

	return ( w );
}


/************************************************
		trapezoid window

	double  *trapezoid(w, leng)
	double	*w;	window values
	int	leng;	window length
************************************************/

double  *trapezoid(w, leng)
	double	*w;
	int	leng;
{
	register int	k, m1, m2;
	register double	*p, slope;

	m1 = leng / 4;
	m2 = (leng * 3) / 4;
	slope = 4.0 / (double)(leng - 1);

	for (k = 0, p = w; k < m1; k++)
		*p++ = slope * k;
	for ( ; k < m2; k++)
		*p++ = 1.0;
	for ( ; k < leng; k++)
		*p++ = 4.0 - slope * k;

	return ( w );
}


/************************************************
		rectangular window

	double  *rectangular(w, leng)
	double	*w;	window values
	int	leng;	window length
************************************************/

double  *rectangular(w, leng)
	double	*w;
	int	leng;
{
	register int	k;
	register double	*p;

	for (k = 0, p = w; k < leng; k++)
		*p++ = 1.0;

	return ( w );
}

