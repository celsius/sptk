/************************************************************************
*									*
*	Swap Bytes							*
*									*
*					1996    T.Kobayashi		*
*					1997.3  G.Hirabayashi		*
*									*
*	usage:								*
*		swab [ options ] [ infile ] > stdout			*
*	options:							*
*		-S S     :  start address     			[0]	*
*		-s s     :  start offset number  		[0]	*
*		-E E     :  end address     			[EOF]	*
*		-e e     :  end offset number  			[0]	*
*		+type    :  input data type 			[s]	*
*				s (short)    l (long)			*
*				f (float)    d (double)			*
*	infile:								*
*		data sequence					[stdin] *
*	stdout:								*
*		swapped data sequence					*
*									*
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Command Name  */
char    *cmnd;


/*  Default Values  */
#define START           0
#define SNO             0
#define END             0x7fffffff
#define ENO             0x7fffffff


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - Swap Bytes\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -S S   : start address       [%d]\n",START);
    fprintf(stderr, "       -s s   : start offset number [%d]\n",SNO);
    fprintf(stderr, "       -E E   : end address         [EOF]\n");
    fprintf(stderr, "       -e e   : end offset number   [0]\n");
    fprintf(stderr, "       +type  : input data type     [s]\n");
    fprintf(stderr, "                 s (short)     l (long)\n");
    fprintf(stderr, "                 f (float)     d (double)\n");
    fprintf(stderr, "       -h     : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       swapped data sequence\n");
    fprintf(stderr, "\n");

    exit(status);
}

static long	start = START, _end = END, sno = SNO, eno = ENO;

main(argc, argv)
int argc;
char *argv[];
{
    FILE	*fp = stdin;
    char	*s;
    int		c, iosize = 2;
    long	atol();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    switch(c) {
		case 'S':
		    start = atol(*++argv);
		    --argc;
		    break;
		case 's':
		    sno = atol(*++argv);
		    --argc;
		    break;
		case 'E':
		    _end = atol(*++argv);
		    --argc;
		    break;
		case 'e':
		    eno = atol(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else if (*s == '+') {
	    c = *++s;
	    switch(c) {
		case 's':
		    iosize = sizeof(short);
		    break;
		case 'l':
		    iosize = sizeof(long);
		    break;
		case 'f':
		    iosize = sizeof(float);
		    break;
		case 'd':
		    iosize = sizeof(double);
		    break;
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n",
			    cmnd, *(*argv+1));
		    usage(1);
	    }
	}
	else
	    fp = getfp(*argv, "r");
	
    conv(fp, iosize);
    exit(0);
}

conv(fp, iosize)
FILE	*fp;
int	iosize;
{
	long	adrs, n;
	int	i;
	char	ibuf[8], obuf[8];

	if(ffseek(fp, adrs = start + iosize * sno))
			return;

	for(n = sno; adrs <= _end && n <= eno; adrs += iosize, ++n) {
		fread(ibuf, iosize, 1, fp);
		if(feof(fp))
			break;
		for(i = 0; i < iosize; ++i)
			obuf[i] = ibuf[iosize - 1 - i];
		fwrite(obuf, iosize, 1, stdout);
	}
}

ffseek(fp, off)
FILE	*fp;
long	off;
{
	register int	n;

	if(fp != stdin)
		fseek(fp, off, 0);
	else {
		for(n = off; n; --n)
			if(getc(fp) == EOF)
				return(1);
	}
	return(0);
}
