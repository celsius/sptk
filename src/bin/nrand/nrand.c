/************************************************************************
*									*
*    Generate Normal Distrubuted Random Value				*
*									*
*					1991.9 T.Kanno			*
*					1996.1 N.Miyazaki modified	*
*									*
*	usage:								*
*		nrand [ options ] > stdout				*
*	options:							*
*		-l l	 : output length		[256]		*
*		-s s     : seed for nrand		[1]		*
*	stdout:								*
*		random value (float)					*
*	notice:								*
*		if l < 0, generate infinite sequence			*
*									*
************************************************************************/

/*  Standard C Libraries  */
#include	<stdio.h>
#include	<math.h>
#include	<string.h>
#include	<SPTK.h>


/*  Default Values  */
#define	RAND_MAX	32767


double	rnd();
unsigned long	srnd();
double	nrandom();
int	nrand();

#define LENG	256
#define SEED	1


/*  Command Name  */
char	*cmnd;


void usage()
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - generate normal distributed random value\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : output length      [%d]\n",LENG);
	fprintf(stderr, "       -s s  : seed for nrand     [%d]\n",SEED);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       random values (float)\n");
	fprintf(stderr, "  notice:\n");
	fprintf(stderr, "       if l < 0, generate infinite sequence\n");
	fprintf(stderr, "\n");
	exit(1);
}


main( argc, argv )
	int 	argc;
	char	*argv[];
{
	char	*str, flg;
	int	leng = LENG, seed = SEED,  i;
	long	next = SEED;
	double	p;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;

	while (--argc)  {
		if (*(str = *++argv) == '-')  {
			flg = *++str;
			if (*++str == '\0')  {
				str = *++argv;
				argc--;
			}
			switch (flg)  {
			    case 'l':	leng = atoi(str);	break;
			    case 's':	seed = atoi(str);	break;
			    case 'h':	
			    default :	usage();
			}
		} else usage();
		
	}

	if(seed != 1) next = srnd((unsigned)seed);
	
	for(i=0;; i++){
	    p = (double)nrandom(&next);
	    fwritef(&p, sizeof(p), 1, stdout);
	    
	    if(i == leng-1) break;
	}
	exit(0);
}

