/***************************************************************
    $Id: getfp.c,v 1.1 2000/03/01 13:58:55 yossie Exp $

    File Open Function

	FILE	*getfp(name, opt)

	char	name : file name
	char	opt  : file open mode

***************************************************************/

#include	<stdio.h>

FILE *getfp(name, opt)
char *name, *opt;
{
    FILE	*fp;

    if ((fp = fopen(name, opt)) == NULL){
	fprintf(stderr, "Can't open '%s'!\n", name);
	exit(2);
    }
    return (fp);
}
