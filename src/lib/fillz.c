/********************************************************
    $Id$

    Fill Data with Zero

	fillz(ptr, size, nitem)

	xxxx	*ptr  : intput data
	int	size  : size of data type
	int	nitem : data length

*********************************************************/

void fillz(ptr, size, nitem)
char *ptr;
int size, nitem;
{
    register long n;
    
    n = size * nitem;
    while(n--)
	*ptr++ = '\0';
}
