
/****************************************************************
*	XY-Plotter Library 	 				*
*								*
*	Calling sequence :					*
*		plopen(mode);					*
*		_plsend(buf, nbytes);				*
*		_plnorm(x);					*
*		plots(mode);					*
*		plote();					*
*		plot(x, y, z);					*
*		plotr(ip, x, y);				*
*		plota(ip, x, y);				*
*		origin(x, y);					*
*		mode(ltype, lscale);				*
*		chlnmod(_lnmode);				*
*								*
*	Copyright 1985 by T. Kobayashi				*
****************************************************************/

#include "plot.h"

struct co_ord _org = { 0.0, 0.0 };

plots(mode)
int	mode;
{
	if((mode & 0x80) == 0) {
		asciimode();		/* ASCII mode */
		ifclear();		/* Interface CLEAR */
		sleep(1);		/* delay */
		home();			/* HOME */
	}
	term(TRM);
	_org.xo = _org.yo = 0;
}

plote()
{
}

_plnorm(x)
float	x;
{
	return(10.0 * x + 0.5);
}

_cordx(x)
float	x;
{
	return(_plnorm(x + _org.xo));
}

_cordy(y)
float	y;
{
	return(_plnorm(y + _org.yo));
}

static short		_ltype = 0, _lscale = 10;

plot(x, y, z)
float	x, y;
register int	z;
{
	register int	ip;

	if(abs(z) == 2)
		ip = 0;
	else if(abs(z) == 3)
		ip = 1;
	else {
		if(z == 999)
			plote();
		return;
	}
	plota(ip, x, y);
	if(z < 0) {
		_org.xo += x;
		_org.yo += y;
	}
}

plotr(ip, x, y)
float	x, y;
int	ip;
{
	if(ip == 1)
		rmove(_plnorm(x), _plnorm(y));
	else if(ip == 0 || ip == 2) {
		if(ip == 2)
			_chlnmod(1);
		rdraw(_plnorm(x), _plnorm(y));
		if(ip == 2)
			_chlnmod(0);
	}
	else
		return(1);
	return(0);
}

plota(ip, x, y)
float	x, y;
int	ip;
{
	if(ip == 1)
		move(_cordx(x), _cordy(y));
	else if(ip == 0 || ip == 2) {
		if(ip == 2)
			_chlnmod(1);
		draw(_cordx(x), _cordy(y));
		if(ip == 2)
			_chlnmod(0);
	}
	else
		return(1);
	return(0);
}

mode(ltype, lscale)
int	ltype;
float	lscale; 
{
	_ltype  = ltype;
	_lscale = _plnorm(lscale);
}

_chlnmod(lmode)
int	lmode;
{
	if(lmode) {
		line_type(_ltype);
		line_scale(_lscale);
	}
	else
		line_type(0);
}

origin(x, y)
float x, y;
{
	_org.xo = x;
	_org.yo = y;
	plot(0., 0., 3);
}
