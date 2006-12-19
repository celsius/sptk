/*
  ----------------------------------------------------------------
	Speech Signal Processing Toolkit (SPTK): version 3.0
			 SPTK Working Group

		   Department of Computer Science
		   Nagoya Institute of Technology
				and
    Interdisciplinary Graduate School of Science and Engineering
		   Tokyo Institute of Technology
		      Copyright (c) 1984-2000
			All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

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

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>

/*  Required Functions  */
void fdump(register FILE *fp);

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
    fprintf(stderr, "  usage:\n");
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
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}


long	start = START, mod = MODULO;
int	size = sizeof(char), is_int = 0, entry = ENTRY, is_char = 1, ff = 0;
char	adrsfmt = 'd', format[SIZE], form[SIZE];


int main(int argc, char **argv)
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

void fdump(register FILE *fp)
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
			    printf("%ld", u.l);
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

