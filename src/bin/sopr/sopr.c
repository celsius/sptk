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
*    Execute Scalar Operations						*
*									*
*					1990.11 T.Kobayashi		*
*					1996.5  K.Koishida		*
*					2000.5  T.Kobayashi		*
*	usage:								*
*		sopr [ options ] [ infile ] > stdout			*
*	options:							*
*		-a a     :  addition       (in + a)			*
*		-s s     :  subtraction    (in - s)			*
*		-m m     :  multiplication (in * m)			*
*		-d d     :  division       (in / d)			*
*		-ABS     :  absolute       (abs(in))			*
*		-INV     :  inverse        (1 / in)			*
*		-P       :  square         (in * in)			*
*		-R       :  root           (sqrt(in))			*
*		-SQRT    :  root           (sqrt(in))			*
*		-LN      :  logarithm      (log(in))			*
*		-LOG10   :  logarithm      (log10(in))			*
*		-EXP     :  exponential    (exp(in))			*
*		-POW10   :  power of 10    (10^(in))			*
*		-FIX     :  round          ((int)in)			*
*		-UNIT    :  unit step	   (u(in))			*
*		-CLIP    :  clipping       (in * u(in))			*
*		-SIN     :  sin		   (sin(in))                    *
*		-COS     :  cos            (cos(in))                    *
*		-TAN     :  tan            (tan(in))                    *
*		-ATAN    :  atan           (atan(in))                   *
*                                                                       *
*		-r mn    :  read from memory register n(0-9)            *
*		-w mn    :  write to memory register n(0-9)	        *
*                                                                       *
*	infile:								*
*		data sequences (float)					*
*	stdout:								*
*		data sequences after operations				*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>

/*  Required Functions  */
int sopr(FILE *fp);

/*  Command Name  */
char	*cmnd;


/* Default Value  */
#define MEMSIZE		10


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - execute scalar operations\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -a a   : addition            (in + a)\n");
    fprintf(stderr, "       -s s   : subtraction         (in - s)\n");
    fprintf(stderr, "       -m m   : multiplication      (in * m)\n");
    fprintf(stderr, "       -d d   : division            (in / d)\n");
    fprintf(stderr, "       -ABS   : absolute            (abs(in))\n");
    fprintf(stderr, "       -INV   : inverse             (1 / in)\n");
    fprintf(stderr, "       -P     : square              (in * in)\n");
    fprintf(stderr, "       -R     : root                (sqrt(in))\n");
    fprintf(stderr, "       -SQRT  : root                (sqrt(in))\n");
    fprintf(stderr, "       -LN    : logarithm           (log(in))\n");
    fprintf(stderr, "       -LOG10 : logarithm           (log10(in))\n");
    fprintf(stderr, "       -EXP   : exponential         (exp(in))\n");
    fprintf(stderr, "       -POW10 : power of 10         (10^(in))\n");
    fprintf(stderr, "       -FIX   : round               ((int)in)\n");
    fprintf(stderr, "       -UNIT  : unit step           (u(in))\n");
    fprintf(stderr, "       -CLIP  : clipping            (in * u(in))\n");
    fprintf(stderr, "       -SIN   : sin                 (sin(in))\n");
    fprintf(stderr, "       -COS   : cos                 (cos(in))\n");
    fprintf(stderr, "       -TAN   : tan                 (tan(in))\n");
    fprintf(stderr, "       -ATAN  : atan                (atan(in))\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "       -r mn  : read from memory regisiter n\n");
    fprintf(stderr, "       -w mn  : write to memory regisiter n\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "       -h     : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)        [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       data sequence after operations (float)\n");
    fprintf(stderr, "\n");

    exit(status);
}


double	log(), log10(), exp(), sqrt(), pow(), sin(), cos(), tan(), atan();

struct operation {
	char	op[4];
	double	d;
	} *optbl;
int	nopr = 0;

static double	mem[MEMSIZE];

int main(int argc,char *argv[])
{
    FILE	*fp;
    char	*s, c;
    char	*infile = NULL;
    double	atof();
	
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    
    optbl = (struct operation *)calloc(sizeof(struct operation),argc);

    for(; --argc ;){
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    if(islower(c) && *++s == '\0') {
		s = *++argv;
		--argc;
	    }
	    switch(c) {
	        case 'a':
	        case 'd':
	        case 'm':
		case 's':
	        case 'r':
	        case 'w':
		    if(strncmp("dB", s, 2) == 0)
		      optbl[nopr].d = 20 / log(10.0);
		    else if(strncmp("pi", s, 2) == 0)
		      optbl[nopr].d = PI;
		    else if(strncmp("ln", s, 2) == 0)
		      optbl[nopr].d = log(atof(s+2));
		    else if(strncmp("exp", s, 3) == 0)
		      optbl[nopr].d = exp(atof(s+3));
		    else if(strncmp("sqrt", s, 4) == 0)
		      optbl[nopr].d = sqrt(atof(s+4));
		    else if(*s == 'm') {
		      optbl[nopr].d = atoi(s+1);
		      if(c == 'a')
		        c = '+';
		      else if(c == 'd')
		        c = '/';
		      else if(c == 'm')
		        c = '*';
		      else if(c == 's')
		        c = '-';
		    }
		    else
		      optbl[nopr].d = atof(s);
		case 'A':
		case 'C':
		case 'E':
		case 'F':
		case 'I':
		case 'L':
		case 'P':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		    if ((c == 'A') || (c == 'C') || (c == 'L') ||
			(c == 'P') || (c == 'S'))
			strncpy(optbl[nopr].op, s, 4);
		    else
			optbl[nopr].op[0] = c;
		    ++nopr;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, c);
		    usage(1);
		}
	}
	else
	    infile = s;
    }

    if(infile) {
	fp = getfp(infile, "r");
	sopr(fp);
    }
    else
	sopr(stdin);

    exit(0);
}

int sopr(FILE *fp)
{
    double	  x, y;
    register int  k, i;

    while(freadf(&x, sizeof(x), 1, fp) == 1) {
        for(k = 0; k < MEMSIZE; ++k) mem[k] = 0;
	for(k = 0; k < nopr; ++k) {
	    y = optbl[k].d;
	    switch(optbl[k].op[0]) {
	        case 'r':
		    x = mem[(int) y];
		    break;
	        case 'w':
	            mem[(int) y] = x;
	            break;
   	        case '+':
		    x += mem[(int) y];
		    break;
	        case '-':
		    x -= mem[(int) y];
		    break;
	        case '*':
		    x *= mem[(int) y];
		    break;
	        case '/':
		    x /= mem[(int) y];
		    break;
	        case 'a':
		    x += y;
		    break;
		case 's':
		    x -= y;
		    break;
		case 'm':
		    x *= y;
		    break;
		case 'd':
		    x /= y;
		    break;
		case 'A':
		    if(optbl[k].op[1] == 'T')
			x = atan(x);
		    else
			if(x < 0) x = -x;
		    break;
		case 'C':
		    if(optbl[k].op[1] == 'L'){
		    	if(x < 0)
				x = 0;
		    }
		    else
			x = cos(x);
		    break;
		case 'I':
		    x = 1 / x;
		    break;
		case 'P':
		    if(optbl[k].op[1] == 'O')
			x = pow(10.0,x);
		    else
		        x *= x;
		    break;
		case 'R':
		    x = sqrt(x);
		    break;
		case 'S':
		    if(optbl[k].op[1] == 'Q')
			x = sqrt(x);
		    else
			x = sin(x);
		    break;
		case 'E':
		    x = exp(x);
		    break;
		case 'L':
		    if(optbl[k].op[3] == '1')
			x = log10(x);
		    else
			x = log(x);
		    break;
		case 'F':
		    if(x < 0)
			i = x - 0.5;
		    else
			i = x + 0.5;
		    x = i;
		    break;
		case 'T':
		    x = tan(x);
		    break;
		case 'U':
		    if(x < 0)
			x = 0;
		    else
			x = 1;
		default:
		    break;
		}
	}
	fwritef(&x, sizeof(x), 1, stdout);
    }
	return(0);
}
