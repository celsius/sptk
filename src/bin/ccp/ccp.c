/************************************************************************
*									*
*    File Copy								*
*									*
*					1991    T.Kobayashi		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		ccp [options] [infile] > stdout				*
*	options:							*
*		-S S     :  start address     			[0]	*
*		-s s     :  start offset number  		[0]	*
*		-E E     :  end address     			[EOF]	*
*		-e e     :  end offset number  			[0]	*
*		+type1   :  input data type 			[f]	*
*		+type2   :  output data type 			[type1]	*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*				a (ascii)				*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define START		0
#define SNO		0
#define END		0x7fffffff
#define ENO		0x7fffffff


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - file copy\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -S S   : start address       [%d]\n",START);
    fprintf(stderr, "       -s s   : start offset number [%d]\n",SNO);
    fprintf(stderr, "       -E E   : end address         [EOF]\n");
    fprintf(stderr, "       -e e   : end offset number   [0]\n");
    fprintf(stderr, "       +type1 : input data type     [f]\n");
    fprintf(stderr, "       +type2 : output data type    [type1]\n");
    fprintf(stderr, "                 c (char)      s (short)\n");
    fprintf(stderr, "                 i (int)       l (long)\n");
    fprintf(stderr, "                 f (float)     d (double)\n");
    fprintf(stderr, "                 a (ascii)\n");
    fprintf(stderr, "       -h     : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       copied data sequence\n");
    fprintf(stderr, "\n");

    exit(status);
}


static long	start = START, _end = END, sno = SNO, eno = ENO;


void main(int argc, char **argv)
{
    char        type[2], *s, c;
    double      x;
    int         size1 = 0, size2 = 0, i, col = 1;
    long        xscanf();
    FILE	*fp = stdin;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (*(s = *++argv) == '+') {
	    (*argv)++;
	    while(**argv != '\0'){
		if(isdigit(**argv))
		    col = atoi(*argv);
		else
		    switch (**argv) {
			case 's':
			    if(size1 == 0){
				type[0] = 's';
				size1 = sizeof(short);
			    } else {
				type[1] = 's';
				size2 = sizeof(short);
			    }
			    break;
			case 'i':
			    if(size1 == 0){
				type[0] = 'i';
				size1 = sizeof(int);
			    } else {
				type[1] = 'i';
				size2 = sizeof(int);
			    }
				break;
			case 'l':
			    if(size1 == 0){
				type[0] = 'l';
				size1 = sizeof(long);
			    } else {
				type[1] = 'l';
				size2 = sizeof(long);
			    }
			    break;
			case 'f':
			    if(size1 == 0){
				type[0] = 'f';
				size1 = sizeof(float);
			    } else {
				type[1] = 'f';
				size2 = sizeof(float);
			    }
			    break;
			case 'd':
			    if(size1 == 0){
				type[0] = 'd';
				size1 = sizeof(double);
			    } else {
				type[1] = 'd';
				size2 = sizeof(double);
			    }
			    break;
			case 'c':
			    if(size1 == 0){
				type[0] = 'c';
				size1 = sizeof(char);
			    } else {
				type[1] = 'c';
				size2 = sizeof(char);
			    }
			    break;
			case 'a':
			    if(size1 == 0){
				type[0] = 'a';
				size1 = -1;
			    } else {
				type[1] = 'a';
				size2 = -1;
			    }
			    break;
			default:
			    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
			    usage(1);
			}
		(*argv)++;
	    }
	}
	else if(*s == '-'){
	    c = *++s;
	    switch (c) {
                case 'S':
		    start = xscanf(*++argv);
		    argc--;
		    break;
                case 's':
		    sno = xscanf(*++argv);
		    argc--;
		    break;
                case 'E':
		    _end = xscanf(*++argv);
		    argc--;
		    break;
                case 'e':
		    eno = xscanf(*++argv);
		    argc--;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else 
	    fp = getfp(*argv, "r");
    
    if(size1 == 0){
	size1 = sizeof(float);
	type[0] = 'f';
    }
    if(size2 == 0){
	size2 = size1;
	type[1] = type[0];
    }
    
    copy(fp, type, size1, size2);
    exit(0);
}

long	xscanf(s)
char	*s;
{
    long	i;

    if(s[0] == '0' && s[1] == 'x')
	sscanf(s + 2, "%lx", &i);
    else
	sscanf(s, "%ld", &i);

    return(i);
}

copy(fp, type, isize, osize)
FILE	*fp;
char	*type;
int	isize, osize;
{
    register long	adrs, n;
    double	ib[1], ob[1], atof();
    char	s[256];

    if(type[0] == 'a') {
	if(ffseek(fp, adrs = start))
	    return;
	for(n = sno; n; --n)
	    adrs += getstr(fp, s);
    }
    else
	if(ffseek(fp, adrs = start + isize * sno))
	    return;

    for(n = sno; adrs <= _end && n <= eno; adrs += isize, ++n) {
	if(type[0] == 'a') {
	    isize = getstr(fp, s);
	    if(isize)
		ib[0] = atof(s);
	}
	else
	    fread(ib, isize, 1, fp);
	if(feof(fp))
	    break;
	if(type[0] == type[1]) {
	    if(type[0] == 'a')
		fwrite(s, sizeof(*s), isize, stdout);
	    else
		fwrite(ib, isize, 1, stdout);
	}
	else {
	    conv(ib, ob, type);
	    if(type[1] == 'a')
		fprintf(stdout, "%lg\n", ob[0]);
	    else
		fwrite(ob, osize, 1, stdout);
	}
    }
}

getstr(fp, s)
FILE	*fp;
register char	*s;
{
    register int	c, i;

    for(i = 0; (c = getc(fp)) == ' ' ||
	c == '\t' || c == '\n' || c == '\r'; ++i)
	;
    
    if(c == EOF)
	return(i);
    for(*s++ = c, ++i; (c = getc(fp)) != EOF && c != ' ' && 
	c != '\t' && c != '\n' && c != '\r'; ++i)
	*s++ = c;

    *s = '\0';
    return(++i);
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

conv(in, out, type)
double	*in, *out;
char	*type;
{
    double	d;

    switch(type[0]) {
	case 's':
	    d = *(short *)in;
	    break;
	case 'i':
	    d = *(int *)in;
	    break;
	case 'l':
	    d = *(long *)in;
	    break;
	case 'f':
	    d = *(float *)in;
	    break;
	case 'a':
	case 'd':
	    d = *(double *)in;
	    break;
	case 'b':
	    d = *(char *)in & 0xff;
	    break;
	case 'c':
	default:
	    d = *(char *)in;
	    break;
    }
    
    switch(type[1]) {
        case 's':
	    *(short *)out = d;
	    break;
        case 'i':
	    *(int *)out = d;
	    break;
	case 'l':
	    *(long *)out = d;
	    break;
	case 'f':
	    *(float *)out = d;
	    break;
	case 'a':
	case 'd':
	    *(double *)out = d;
	    break;
	case 'b':
	case 'c':
	default:
	    *(char *)out = d;
	    break;
    }
}

size(c)
int	c;
{
    int	nbytes;

    switch(c) {
        case 'a':
	case 'b':
	case 'c':
	    nbytes = 1;
	    break;
	case 'w':
	    nbytes = 2;
	    break;
	case 'l':
	case 'f':
	    nbytes = 4;
	    break;
	case 'd':
	    nbytes = 8;
	    break;
	default:
	    fprintf(stderr, "copy: unknown data type '%c'\n", c);
	    exit(1);
	    break;
    }
    return(nbytes);
}


