/*
	freadf  : read float type datas and convert to double type.

	fwritef : convert double type datas to float type and write.
								*/


#include <stdio.h>
#include <SPTK.h>
static	float	*f;
static	int	items;


#ifndef DOUBLE

int	fwritef(ptr, size, nitems, fp)
double	*ptr;
unsigned int	size;
int	nitems;
FILE	*fp;
{
	int	i;
	if (items < nitems) {
		if (f != NULL)
			free(f);
		items = nitems;
		f = fgetmem(items);
	}
	for (i=0; i<nitems; i++)
		f[i] = ptr[i];
	return fwrite(f, sizeof(float), nitems, fp);
}

int	freadf(ptr, size, nitems, fp)
double	*ptr;
int	size;
int	nitems;
FILE	*fp;
{
	int	i, n;
	if (items < nitems) {
		if (f != NULL)
			free(f);
		items = nitems;
		f = fgetmem(items);
	}
	n = fread(f, sizeof(float), nitems, fp);
	for (i=0; i<n; i++)
		ptr[i] = f[i];
	return n;
}

#endif	/* DOUBLE */

