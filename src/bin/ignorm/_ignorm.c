/************************************************************************
  $Id: _ignorm.c,v 1.1.1.1 2000/03/01 13:58:27 yossie Exp $

    Inverse Gain Normalization
	
	void 	ignorm(c1, c2, m, g)

	double	*c1 :  normalized generalized cepstral coefficients
	double	*c2 :  genenralized cepstral coefficients
	int	  m :  order of coefficients
	double	  g :  gamma

************************************************************************/

void ignorm(c1, c2, m, g)
double	*c1, *c2, g;
int	m;
{
    double	k, log(), pow();
	
    if (g != 0.0) {
	k = pow(c1[0], g);
	for ( ; m >= 1; m--)
	    c2[m] = k * c1[m];
	c2[0] = (k - 1.0) / g;
    }
    else{
	movem(&c1[1], &c2[1], sizeof(*c1), m);
	c2[0] = log(c1[0]);
    }
}
