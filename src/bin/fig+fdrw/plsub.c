/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/****************************************************************
*	XY PLOTTER library					*
*								*
*	Calling sequence :					*
*		factor(fx, fy);					*
*		rotate(th);					*
*		offset(x, y);					*
*		bound(xl, yl, xh, yh);				*
*		hatch(ip, x, y, n, d, t);			*
*		pen(ip);					*
*		speed(isp);					*
*		mark(mrk, ax, ay, n, f[, m]);			*
*		symbol(x, y, text, h, s, th)			*
*		number(x, y, fval, h, s, th, m, n);		*
*		italic(th)					*
*		font(n)						*
*		line(ip, x, y, n);				*
*		circle(x, y, rs, re, ths, the)			*
*		rcircle(rs, re, ths, the);			*
*								*
*	Copyright 1985 by T. Kobayashi				*
****************************************************************/

#include "plot.h"

extern struct co_ord	_org;

rotate(th)
float	th;
{
	_rotate(_plnorm(_org.xo), _plnorm(_org.yo), _plnorm(th));
}

factor(fx, fy)
float	fx, fy;
{
	_factor((int)(fx * SCALE + 0.5), (int)(fy * SCALE + 0.5), SCALE);
}

offset(x, y)
float	x, y;
{
	_offset(_plnorm(x), _plnorm(y));
	return(0);
}

bound(xl, yl, xh, yh)
float	xl, yl, xh, yh;
{
	short	buf[7];

	wlo_right(_cordx(xl), _cordy(yl));
	wup_left(_cordx(xh), _cordy(yh));
}

rstbnd()
{
	wlo_right(0, 0);
	wup_left(4000, 2850);
}

hatch(ip, ax, ay, n, d, t)
int	ip, n;
float	d, t;
register float	*ax, *ay;
{
	if(n < 3)
		return(1);

	ip += 20;
	_hatch(ip, _plnorm(d), _plnorm(t));

	while(--n >= 0)
		sndcord(_cordx(*ax++), _cordy(*ay++));
	terminate();
	return(0);
}

pen(pen)
int	pen;
{
	if(pen < 1 || pen > 10)
		return(1);
	newpen(pen);
	return(0);
}

font(n)
int	n;
{
	if(n < 0 || n > 19)
		return(1);
	_font(n);
	return(0);
}

speed(isp, ip)
int	isp, ip;
{
	if(isp < 0 || isp > 10 || ip < 0 || ip > 10)
		return(1);
	if(ip)
		_speed_all(isp);
	else
		_speed(isp, ip);
	return(0);
}

mark(mrk, ax, ay, n, f, m)
int	mrk, n, m;
float	ax[], ay[];
float	f;
{
	register int	i;

	if(mrk < 0 || mrk > 15)
		return(1);
	m = (n < 0) ? m : 1;
	if((n = abs(n)) < 1 || m < 0)
		return(1);
	ascale(_plnorm(f * 1.75));
	arotate(0);
	for(i = 0; i < n; i += m) {
		plot(ax[i], ay[i], 3);
		_mark(mrk);
	}
	return(0);
}

symbol(x, y, text, h, s, th)
float	x, y, h, s, th;
char	*text;
{
	plot(x, y, 3);
	ascale(_plnorm(h));
	aspace(_plnorm(s));
	arotate(_plnorm(th));
	print(text);
	return(0);
}

number(x, y, fval, h, s, th, m, n)
float	x, y, h, s, fval, th;
int	m, n;
{
	char	buf[32], format[8];

	if(abs(m) > 10)
		return(1);
	if(n <= 0) {
		while(++n < 0)
			fval /= 10.0;
		sprintf(format, "%%%dd%s", m, (n) ? ".\r" : "\r");
		sprintf(buf, format, (long)fval);
	}
	else {
		sprintf(format, "%%%d.%df\r", m, n);
		sprintf(buf, format, fval);
	}
	return(symbol(x, y, buf, h, s, th));
}

italic(th)
float	th;
{
	int	theta;

	if((theta = 256 * tan(th * DEG_RAD)) > 4000)
		return(1);
	aitalic(theta);
	return(0);
}

line(ip, ax, ay, n)
int	ip, n;
register float	*ax, *ay;
{
	struct {
		short	x;
		short	y;
	} b, o, pb;
	register int	dx, dy;

	if(n < 2)
		return(1);
	if(ip)
		_chlnmod(1);
	plot(*ax++, *ay++, 3);
	o.x = dx = _cordx(*ax++);
	o.y = dy = _cordy(*ay++);
	pb = o;
	_draw();

	while(--n > 1) {
		dx = (b.x = _cordx(*ax++)) - pb.x;
		dy = (b.y = _cordy(*ay++)) - pb.y;
		if(dx || dy) {
			if(dx == 0 && o.x == 0 && sign(dy) == sign(o.y))
				pb.y = b.y;
			else if(dy == 0 && o.y == 0 && sign(dx) == sign(o.x))
				pb.x = b.x;
			else {
				sndcord(pb.x, pb.y);
				pb.x = b.x;
				pb.y = b.y;
			}
			o.x = dx;
			o.y = dy;
		}
	}
	sndcord(pb.x, pb.y);
	terminate();
	if(ip)
		_chlnmod(0);
	return(0);
}

circle(x, y, rs, re, ths, the)
float	x, y, rs, re, ths, the;
{
	int	r1, r2;

	r1 = _plnorm(rs);
	r2 = _plnorm(re);
	if(r1 == 0 && r2 == 0)
		return(1);
	_circle(_cordx(x), _cordy(y), r1, r2, _plnorm(ths), _plnorm(the));
	return(0);
}

pntstyl(ip)
int	ip;
{
	ip += 20;
	aspace(ip);	/* valid for only LBP */
	return(0);
}
