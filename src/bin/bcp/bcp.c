/************************************************************************
*									*
*    Block Copy								*
*									*
*					1988.7  T.Kobayashi		*
*					1996.5  K.Koishida		*
*									*
*	usage:								*
*		bcp [options] [infile] > stdout				*
*	options:							*
*		-l l     :  number of items contained 1 block	[512]	*
*		-L L     :  number of destination block size	[N/A]	*
*		-n n     :  order of items contained 1 block	[l-1]	*
*		-N N     :  order of destination block size	[N/A]	*
*		-s s     :  start number                        [0]	*
*		-S S     :  start number in destination block   [0]     *
*		-e e     :  end number                          [EOF]	*
*		-f f     :  fill into empty block               [0.0]   *
*		+type    :  data type 				[f]	*
*				c (char)     s (short)			*
*				i (int)      l (long)			*
*				f (float)    d (double)			*
*				a (ascii)				*
*	infile:								*	
*		data sequence					[stdin]	*
*	stdout:								*
*		copied data sequence					*
*	note:								*
*		When both (-l and -n) or (-L and -N) are specified,	*
*		latter argument is adopted.				*
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
#define END		-1
#define DSTART		0
#define ITEM		512
#define DITEM		0
#define FILL		0.0

/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - block copy\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : number of items contained 1 block      [%d]\n",ITEM);
    fprintf(stderr, "       -L L  : number of destination block size       [N/A]\n",DITEM);
    fprintf(stderr, "       -n n  : order of items contained 1 block       [l-1]\n");
    fprintf(stderr, "       -N N  : order of destination block size        [N/A]\n",DITEM-1);
    fprintf(stderr, "       -s s  : start number                           [%d]\n",START);
    fprintf(stderr, "       -S S  : start number in destination block      [%d]\n",DSTART);
    fprintf(stderr, "       -e e  : end number                             [EOF]\n");
    fprintf(stderr, "       -f f  : fill into empty block                  [%g]\n",FILL);
    fprintf(stderr, "       +type : data type                              [f]\n");
    fprintf(stderr, "                c (char)      s (short)\n");
    fprintf(stderr, "                i (int)       l (long)\n");
    fprintf(stderr, "                f (float)     d (double)\n");
    fprintf(stderr, "                a (ascii)\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                                  [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       copied data sequence\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       When both (-L and -N) or (-l and -n) are specified,\n");
    fprintf(stderr, "       latter argument is adopted.\n");
    fprintf(stderr, "\n");

    exit(status);
}

int	sno = START, eno = END, dsno = DSTART,
        size = sizeof(float), nitems = ITEM, dnitems = DITEM;
double	fl=FILL;
char type='f';
union typex{
    char c;
    short s;
    long l;
    int i;
    float f;
    double d;
}fillx;

void main(int argc, char **argv)
{
    FILE 	   *fp = stdin;
    register char  *s, c;
    double atof();
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    switch(c) {
	        case 'n':
		    nitems = atoi(*++argv)+1;
		    --argc;
		    break;
		case 'N':
		    dnitems = atoi(*++argv)+1;
		    --argc;
		    break;
	        case 'b':
		    nitems = atoi(*++argv);
		    --argc;
		    break;
	        case 'l':
		    nitems = atoi(*++argv);
		    --argc;
		    break;
		case 'B':
		    dnitems = atoi(*++argv);
		    --argc;
		    break;
		case 'L':
		    dnitems = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    sno = atoi(*++argv);
		    --argc;
		    break;
		case 'S':
		    dsno = atoi(*++argv);
		    --argc;
		    break;
		case 'e':
		    eno = atoi(*++argv) + 1;
		    --argc;
		    break;
		case 'f':
		    fl = atof(*++argv);
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
	    type = *++s;
	    switch(type) {
		case 'a':
		    size = 0;
		    break;
		case 'c':
		    size = sizeof(char);
		    break;
		case 's':
		    size = sizeof(short);
		    break;
		case 'l':
		    size = sizeof(long);
		    break;
		case 'i':
		    size = sizeof(int);
		    break;
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
	else
	    fp = getfp(*argv, "r");

    if(eno < 0)
	eno = nitems;
    if(sno < 0 || sno >= nitems || sno > eno || eno > nitems || dsno < 0)
	exit(1);

    if(dnitems == 0)
	dnitems = eno - sno + dsno;

    bcp(fp);
    
    exit(0);
}

bcp(fp)
FILE	*fp;
{
    char	*buf, *lz, *fz;
    int	ibytes, obytes, offset, nlz, nfz;
    switch (type) {
	case 'c':
	    fillx.c = (char)fl;
	    break;
	case 's':
	    fillx.s = (short)fl;
	    break;
	case 'l':
	    fillx.l = (long)fl;
	    break;
	case 'i':
	    fillx.i = (int)fl;
	    break;
	case 'f':
	    fillx.f = (float)fl;
	    break;
	case 'd':
	    fillx.d = (double)fl;
	    break;
	case 'a':
	    break;
    }
    
    if(size == 0) {
	acopy(fp);
	return;
    }

    ibytes = size * nitems;
    offset = size * sno;
    obytes = size * (eno - sno);
    nlz    = size * dsno;
    nfz    = ((nfz = size * dnitems - nlz - obytes) < 0) ? 0 : nfz;
    if((buf = (char *)dgetmem(ibytes + nlz + nfz)) == NULL)
	return;

    if(nlz) {
	lz = buf + ibytes;
	filln(lz, size, nlz);
    }

    if(nfz) {
	fz = buf + ibytes + nlz;
	filln(fz, size, nfz);
    }

    while(fread(buf, sizeof(*buf), ibytes, fp) == ibytes) {
	if(nlz)
	    fwrite(lz, sizeof(*lz), nlz, stdout);
	fwrite(buf + offset, sizeof(*buf), obytes, stdout);
	
	if(nfz)
	    fwrite(fz, sizeof(*fz), nfz, stdout);
    }
}

acopy(fp)
FILE	*fp;
{
    char	s[512];
    register int	n, dn;

    for(dn = 0; !feof(fp); ) {
	for(n = 0; n < sno; ++n)
	    if(getstr(fp, s) == 0) break;
	
	for( ; n < eno; ++n) {
	    if(getstr(fp, s) == 0)
		break;
	    if(dn++)
		putchar(' ');
	    printf("%s", s);
	    if(dn == dnitems) {
		putchar('\n');
		dn = 0;
	    }
	}
	for( ; n < nitems; ++n)
	    if(getstr(fp, s) == 0)
		break;
    }
}

getstr(fp, s)
FILE	*fp;
register char	*s;
{
    register int	c;
    
    while((c = getc(fp)) == ' ' || c == '\t' || c == '\n' || c == ',');
    
    if(c == EOF)
	return(0);
    for(*s++ = c; (c = getc(fp)) != EOF && c != ' ' && c != '\t' && c != '\n' && c != ','; )
	*s++ = c;
    
    *s = '\0';
    return(1);
}

filln(ptr, size, nitem)
char *ptr;
int size, nitem;
{
    register long n;
    register int i;
    char *c;
    
    nitem=nitem/size;
    while(nitem--){
        c = &fillx.c;
	for(i=1;i<=size;i++){
	    *ptr++ = *c++;
        }
    }
}
