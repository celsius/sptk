/***************************************************************
    $Id: _mc2b.c,v 1.1 2000/03/01 13:58:28 yossie Exp $

    Transform Mel Cepstrum to MLSA Digital Filter Coefficients

	void	mc2b(mc, b, m, a)

	double	*mc : mel cepstral coefficients
	double	*b  : MLSA digital filter coefficients
	int	m   : order of mel cepstrum
	double	a   : all-pass constant

***************************************************************/

void mc2b(mc, b, m, a)
double *mc, *b, a;
int m;
{
    b[m] = mc[m];
    
    for(m--; m>=0; m--)
	b[m] = mc[m] - a * b[m+1];
}
