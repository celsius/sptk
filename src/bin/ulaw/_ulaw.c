/************************************************************************
  $Id: _ulaw.c,v 1.1 2000/03/01 13:58:51 yossie Exp $

    u-law coder
	
	double ulaw_c(x, max, mu)

	double	  x :  data
	double	max :  max value
	double	 mu :  compression ratio

	return value	: compressed data

    u-law decoder
	
	double ulaw_d(x, max, mu)

	double	  x :  compressed data
	double	max :  max value
	double	 mu :  compression ratio

	return value	: uncompressed data

************************************************************************/

#define	abs(x)	((x>=0) ? (x) : (-(x)))
#define	sign(x)	((x>=0) ? 1 : (-1))

double ulaw_c(x, max, mu)
double	x, max, mu;
{
    double	y, log();
    
    y = sign(x) * max * log(1 + mu*abs(x)/max) / log(1 + mu);
    return(y);
}

double ulaw_d(x, max, mu)
double	x, max, mu;
{
    double	y, pow();
    
    y = sign(x) * max * (pow(1+mu, abs(x)/max) -1) / mu;
    return(y);
}

