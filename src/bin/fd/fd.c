/************************************************************************
*									*
*    File dump								*
*									*
*					1988    T.Kobayashi		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		fdmp [options] [infile] > stdout			*
*	options:							*
*		-a a     :  address				[0]	*
*		-n n     :  initial value for numbering         [0]	*
*		-m m     :  modulo for numbering		[EOF]   *
*		+type    :  data type 				[c]	*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*		%format  :  print format                        [N/A]	*
*									*
************************************************************************/

static char *rcs_id = "$Id: fd.c,v 1.2 2000/06/17 14:02:34 yossie Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define START		0
#define MODULO		0x7fffffff
#define ENTRY		0
#define SIZE            128


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - file dump\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -a a  : address                     [%d]\n",START);
    fprintf(stderr, "       -n n  : initial value for numbering [%d]\n",START);
    fprintf(stderr, "       -m m  : modulo for numbering        [EOF]\n");
    fprintf(stderr, "       -ent  : number of data in each line [%d]\n",ENTRY);
    fprintf(stderr, "       +type : data type                   [c]\n");
    fprintf(stderr, "                c (char)      s (short)\n");
    fprintf(stderr, "                i (int)       l (long)\n");
    fprintf(stderr, "                f (float)     d (double)\n");
    fprintf(stderr, "       %%form : print format(printf style) [N/A]\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                       [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       dumped data sequence\n");
    fprintf(stderr, "\n");

    exit(status);
}


long	start = START, mod = MODULO;
int	size = sizeof(char), is_int = 0, entry = ENTRY, is_char = 1, ff = 0;
char	adrsfmt = 'd', format[SIZE], form[SIZE];


void main(int argc, char **argv)
{
    FILE 	   *fp = stdin;
    register char  *s, c;
    long	   atol();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if(*(s = *++argv) == '-') {
	    if((c = *++s) > '0' && c <= '9')
		c = 'c';
	    switch(c) {
		case 'a':
		case 'n':
		    adrsfmt = c;
		    start = atol(*++argv);
		    --argc;
		    break;
		case 'm':
		    mod = atol(*++argv);
		    --argc;
		    break;
		case 'c':
		    entry = atoi(s);
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
		case 'b':
		    is_char = 0;
		    break;
		case 'c':
		    size = sizeof(char);
		    break;
		case 's':
		    size = sizeof(short);
		    break;
		case 'l':
		case 'i':
		    is_int = 1;
		case 'f':
		    size = sizeof(float);
		    break;
		case 'd':
		    size = sizeof(double);
		    break;
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
	    }
	}
        else if( *s == '%'){
		strcpy(format,s);
		ff = 1;
	}
	else
	    fp = getfp(*argv, "r");

    fdump(fp);

    exit(0);
}

fdump(fp)
register FILE *fp;
{
    char s[18];
    register long	adrs, i, n;
    union {
	unsigned char	b;
	short	w;
	long	l;
	float	f;
	double	d;
    }u;

    if(ff) strcat(format," ");
    
    if(entry == 0) {
	if(size == 8)
	    entry = 2;
	else if(size > 1)
	    entry = 5;
	else if(is_char)
	    entry = 16;
	else
	    entry = 10;
    }
    
    fread(&u, size, 1, fp);
    while(!feof(fp))
	for(n = adrs = start;n < mod && !feof(fp);adrs += entry * size){
	    if(adrsfmt == 'a')
		printf("%06lx  ", adrs);
	    else if(adrsfmt == 'n')
		printf("%7ld  ", n);

	    for(i = 0; i < entry && !feof(fp) && n < mod; ++i, ++n){
		switch(size) {
		    case 1:
		        s[i] = ((u.b & 0x7f) < 32 || u.b == 0x7f || u.b >= 0xe0) ? '.' : u.b;
			if(is_char)
			    printf("%02x ", u.b);
			else
			    printf("%5d", u.b);
			break;
		    case 2:
			printf("%7d", u.w);
			break;
		    case 4:
			if(is_int)
			    printf("%12d", u.l);
			else if(ff)
			    printf(format, u.f);
			else
			    printf("%14.6e", u.f);
			break;
		    case 8:
			if(ff)
			    printf(format, u.d);
			else
			    printf("%24.15e", u.d);
		    default:
			break;
		}
		fread(&u, size, 1, fp);
	    }
	    if(size == 1 && is_char) {
		s[i] = '\0';
		while(i++ < entry)
		    printf("   ");
		printf("|%s|\n", s);
	    }
	    else
		printf("\n");
	}
}

