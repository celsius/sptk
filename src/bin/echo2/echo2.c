/************************************************************************
*									*
*    	echo arguments to the standard error				*
*									*
*					1997.2  K.Koishida		*
*									*
*	usage:								*
*		echo2 [ options ] [ argument... ]			*
*	options:							*
*		-n  :  do not add the NEWLINE to the output. [FALSE]	*
*									*
************************************************************************/

/* Standard C Libraries */
#include <stdio.h>
#include <string.h>

void main(short argc, char *argv[])
{
    int  nl = 1;

    if(argc > 1){
	if (strcmp(argv[1], "-n") == 0){
	    nl = 0;
	    argc--;
	    argv++;
	}
	
	while (--argc){
	    fputs(*++argv, stderr);
	    if (argc > 1)
		putc(' ', stderr);
	}
	
	if (nl)
	    putc('\n', stderr);
    }
    
    exit(0);
}
