/****************************************************************

    $Id: _lspcheck.c,v 1.1 2000/03/01 13:58:40 yossie Exp $

    Check order of LSP

	int lspcheck(lsp, ord)

	double   	*lsp : LSP
	int    	ord  : order of LSP

	return value : 0 -> normal
		       -1-> ill condition

*****************************************************************/

int lspcheck(lsp, ord)
double *lsp;
int ord;
{
    register int	i;
    
    if(lsp[0] <=0.0 || lsp[0] >= 0.5) 
	return(-1);

    for(i=1; i<ord; i++){
	if(lsp[i] <= lsp[i-1]) 
	    return(-1);
	if(lsp[i] <= 0.0 || lsp[i] >= 0.5)
	    return(-1);
    }
    return(0);
}

/****************************************************************

    $Id: _lspcheck.c,v 1.1 2000/03/01 13:58:40 yossie Exp $

    Rearrangement of LSP

	void lsparng(lsp, ord)

	double   	*lsp : LSP
	int    	ord  : order of LSP

*****************************************************************/

void lsparrange(lsp, ord)
double *lsp;
int ord;
{
    register int	i, flag;
    double 		tmp;
    
    /* check out of range */
    for(i=0; i<ord; i++){
	if(lsp[i] < 0.0) 
	    lsp[i] = -lsp[i];
	if(lsp[i] > 0.5) 
	    lsp[i] = 1.0 - lsp[i];
    }
    
    /* check unmonotonic */
    for(;;){
	flag = 0;
	for(i=1; i<ord; i++)
	    if(lsp[i] < lsp[i-1]){
		tmp = lsp[i];
		lsp[i] = lsp[i-1];
		lsp[i-1] = tmp;
		flag = 1;
	    }

	if(! flag) break;
    }
}

