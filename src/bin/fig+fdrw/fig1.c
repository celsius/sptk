/****************************************************************
* NAME :	fig1 - subroutines for fig			*
****************************************************************/

#include "fig.h"

#define GRKSIZE	62
static char	*grk[]={"Alpha", "Beta", "Gamma", "Delta",
			"Epsilon", "Zeta", "Eta", "Theta",
			"Iota", "Kappa", "Lambda", "Mu",
			"Nu", "Xi", " ", "Pi",
			"Rho", "Sigma", "Tau", "Upsilon",
			"Phi", "Chi", "Psi", "Omega",
			" ", " ", " ", " ", " ", " ", " ", " ",
			"alpha", "beta", "gamma", "delta",
			"epsilon", "zeta", "eta", "theta",
			"iota", "kappa", "lambda", "mu",
			"nu", "xi", " ", "pi",
			"rho", "sigma", "tau", "upsilon",
			"phi", "chi", "psi", "omega",
			" ", " ", " ", " ", " ", "infty"};
static unsigned char	sub[SBUFLNG];
static int	nsub;

char	*gettxt(s)
register char	*s;
{
	register char	*p;
	int	i, c;
	char	*script();

	while(*s && *s != '"')
		++s;
	if(*s++ == '\0')
		return(s - 1);
	for(p = s, nsub = i = 0; *s && (*s != '"' || *(s + 1) == '"'); ++s) {
		if(*s == '\\') {		/* escape sequence */
			if((c = greek(s + 1)) >= 0) {
				s += strlen(grk[c]);
				c += 193;
			}
			else {
				switch (c = *++s) {
				case 'b':
					c = 0x08;
					break;
				case 'n':
					c = 0x0a;
					break;
				case 'r':
					c = 0x0d;
					break;
				case '^':
					c = '^';
					break;
				case '_':
					c = '_';
					break;
				case '\\':
					c = '\\';
					break;
				case 'd':
				case 'u':
					s = script(s, i);
					c = ' ';
					break;
				default:
					sscanf(s, "%3d", &c);
					s += 2;
					break;
				}
			}
			p[i++] = c;
		}
		else if((c = *s) == '^' || c == '_') {
			if(*(s + 1) == '{') {
				for(*++s = c; *(s + 1) != '}'; p[i++] = ' ') {
					s = script(s, i);
					*s = c;
				}
				++s;
			}
			else {
				s = script(s, i);
				p[i++] = ' ';
			}
		}
		else {
			if(*s == '"')
				++s;
			p[i++] = *s;
		}
	}
	sub[nsub] = p[i] = '\0';
	while(p + i < s)
		p[++i] = ' ';
	return(p);
}

char	*script(s, i)
register char	*s;
register int	i;
{
	int	c;

	sub[nsub++] = *s;
	sub[nsub++] = i;
	if((c = *++s) == '\\') {
		if((c = greek(s + 1)) >= 0) {
			s += strlen(grk[c]);
			c += 193;
		}
		else {
			switch(c = *++s) {
			case '\\':
			case '{':
			case '}':
			case '_':
			case '^':
				break;
			default:
				sscanf(s, "%3d", &c);
				s += 2;
			break;
			}
		}
	}
	sub[nsub++] = c;
	sub[nsub++] = '\0';
	return(s);
}

char	*getarg(s, arg)
register char	*s, *arg;
{
	if(s == NULL)
		return(s);
	while(*s == ' ' || *s == '\t' || *s == '\n')
		++s;
	if(*s == '\0')
		return(NULL);
	else if(*s == '"') {
		gettxt(s);
		while(*arg++ = *s++)
			;
	}
	else {
		while(*s != ' ' && *s != '\t' && *s != '\n' && *s != '\0')
			*arg++ = *s++;
		*arg = '\0';
	}
	return(s);
}

char	*gettyp(s, t)
register char	*s, *t;
{
	char	*p;

	s = getarg(p = s, t);
	if(isalpha(*t))
		return(s);
	else {
		strcpy(t, "lin");
		return(p);
	}
}

char	*getname(s, t)
register char	*s, *t;
{
	if((s = getarg(s, t)) == NULL)
		*t = '\0';
	else if(*t == '"')
		*t++ = '\0';
	else {
		if(!is_number(*t))
			*t = '\0';
		t = gettxt(s);
	}
	return(t);
}

greek(p)
char	*p;
{
	register int	n;

	for(n = 0; n < GRKSIZE; ++n)
		if(strncmp(p, grk[n], strlen(grk[n])) == 0)
			return(n);
	return(-1);
}

float	sleng(p, h, w)
char	*p;
float	h, w;
{
	return((strlen(p) - 1) * w + LADJ * w);
}

_symbol(x, y, p, h, w, t)
float	x, y, h, w, t;
char	*p;
{
	register int	i;
	float	dx, dy, rx(), ry();

	symbol(x, y, p, h, w, t);
	for(i = 0; sub[i]; i += 4) {
		dy = (sub[i] == 'd' || sub[i] == '_') ? -h * 0.25 : h * 0.7;
		dx = sub[i + 1] * w;
		symbol(x + rx(dx, dy, t), y + ry(dx, dy, t),
				sub + i + 2, h * SSIZE, w, t);
	}
	sub[0] = 0;
}

float	ysadj()
{
	int	i;

	for(i = 0; sub[i]; i += 4)
		if(sub[i] == 'u' || sub[i] == '^')
			return(1.5);

	return(0);
}

float	rx(x, y, t)
float	x, y, t;
{
	t *= (3.141592653589793 / 180);
	return(x * cos(t) - y * sin(t));
}

float	ry(x, y, t)
float	x, y, t;
{
	t *= (3.141592653589793 / 180);
	return(x * sin(t) + y * cos(t));
}

#define	PI	3.141592653589793
#include <math.h>

float	argapf(x, a)
float	x, a;
{
	float	omg;

	omg = PI * x; 
	return(x + 2 * atan2(a * sin(omg), 1 - a * cos(omg)) / PI);
}
