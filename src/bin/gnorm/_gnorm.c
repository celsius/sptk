/************************************************************************
  $Id: _gnorm.c,v 1.1.1.1 2000/03/01 13:58:27 yossie Exp $

    Gain Normalization
	
	void gnorm(c1, c2, m, g)

	double	*c1 :  generalized cepstral coefficients
	double	*c2 :  normalized generalized cepstral coefficients
	int	  m :  order of coefficients
	double	  g :  gamma

************************************************************************/

void gnorm(c1, c2, m, g)
double	*c1, *c2, g;
int	m;
{
    double	k, pow(), exp();
    
    if (g != 0.0) {
	k = 1.0 + g * c1[0];
	for ( ; m >= 1; m--)
	    c2[m] = c1[m] / k;
	c2[0] = pow(k, 1.0/g);
    }
    else{
	movem(&c1[1], &c2[1], sizeof(*c1), m);
	c2[0] = exp(c1[0]);
    }
}
