/***************************************************************
    $Id: _b2mc.c,v 1.1 2000/03/01 13:58:28 yossie Exp $

    Transform MLSA Digital Filter Coefficients to Mel Cepstrum 

	void	b2mc(b, mc, m, a)

	double	*b  : MLSA digital filter coefficients
	double	*mc : mel cepstral coefficients
	int	m   : order of mel cepstrum
	double	a   : all-pass constant

***************************************************************/

void b2mc(b, mc, m, a)
double *b, *mc, a;
int m;
{
    double d, o;
    
    d = mc[m] = b[m];
    for(m--; m>=0; m--){
	o = b[m] + a * d;
	d = b[m];
	mc[m] = o;
    }
}
