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
*    Execute Vector Operations						*
*									*
*					1988.6  T.Kobayashi		*
*					1996.5  K.Koishida		*
*	usage:								*
*		vopr [ options ] [ [ file1 ] [ file2 ] ] > stdout	*
*	options:							*
*		-l l     :  length of vector			[1]	*
*		-i       :  specified file contains a and b	[FALSE]	*
*		-a       :  addition       (a + b)			*
*		-s       :  subtraction    (a - b)			*
*		-m       :  multiplication (a * b)			*
*		-d       :  division       (a / b)			*
*		-ATAN2   :  atan2          atan2(b,a)			*
*	notice:								*
*		file1    : vector a. stdin for default			*
*		file2    : vector b. stdin for default			*
*									*
************************************************************************/

static char *rcs_id = "$Id: vopr.c,v 1.2 2002/12/25 05:33:08 sako Exp $";

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG		1
#define INV		FA


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - excute vector operations\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n", cmnd);
    fprintf(stderr, "       %s [ options ] [ file1 ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l   : length of vector                [%d]\n",LENG);
    fprintf(stderr, "       -n n   : order of vector                 [l-1]\n",LENG);
    fprintf(stderr, "       -i     : specified file contains a and b [%s]\n",BOOL[INV]);
    fprintf(stderr, "       -a     : addition       (a + b)\n");
    fprintf(stderr, "       -s     : subtraction    (a - b)\n");
    fprintf(stderr, "       -m     : multiplication (a * b)\n");
    fprintf(stderr, "       -d     : division       (a / b)\n");
    fprintf(stderr, "       -ATAN2 : atan2          atan2(b,a)\n");
    fprintf(stderr, "       -h     : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data vectors (float)                     [stdin]\n");
    fprintf(stderr, "  file1:\n");
    fprintf(stderr, "       data vectors (float)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       data vectors after operations (float)\n");
    fprintf(stderr, "  note:\n");
    fprintf(stderr, "       When both -l and -n are specified,\n");
    fprintf(stderr, "       latter argument is adopted.\n");
    fprintf(stderr, "\n");
    exit(status);
}

double  atan2();

int 	 opr = ' ', leng = LENG;
Boolean  inv = INV;

void main(int argc, char **argv)
{
    int	     nfiles = 0;
    FILE     *fp1, *fp2;
    char     *s, c, *infile[4];

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while(--argc) {
	if(*(s = *++argv) == '-') {
	    c = *++s;
	    if(*++s == '\0' && (c == 'l' || c == 'n')) {
		s = *++argv;
		--argc;
	    }
	    switch(c) {
	        case 'l':
		    leng = atoi(s);
		    break;
		case 'n':
		    leng = atoi(s)+1;
		    break;
		case 'i':
		    inv = 1 - inv;
		    break;
		case 'a':
		case 'd':
		case 'm':
		case 's':
		case 'A':
		    opr = c;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
            }
	}
	else
	    infile[nfiles++] = s;
    }

    if(nfiles == 0)
	vopr(stdin, stdin);
    else {
	fp1 = getfp(infile[0], "r");
	if(nfiles == 1) {
	    if(inv)
		vopr(fp1, fp1);
	    else
		vopr(stdin, fp1);
	}
	else {
	    fp2 = getfp(infile[1], "r");
	    vopr(fp1, fp2);
	}
    }

    exit(0);
}

vopr(fp1, fp2)
FILE	*fp1, *fp2;
{
    double        *a, *b;
    register int  k;

    a = dgetmem(leng + leng);
    b = a + leng;
    
    if(fp1 != fp2 && leng > 1) {
	if(freadf(b, sizeof(*b), leng, fp2) != leng)
	    return(1);
    }
    while(freadf(a, sizeof(*a), leng, fp1) == leng) {
	if(fp1 == fp2 || leng == 1) {
	    if(freadf(b, sizeof(*b), leng, fp2) != leng)
		return(1);
	}

	switch(opr) {
  	    case 'a':
	        for(k = 0; k < leng; ++k)
		    a[k] += b[k];
		break;
	    case 's':
		for(k = 0; k < leng; ++k)
		    a[k] -= b[k];
		break;
	    case 'm':
		for(k = 0; k < leng; ++k)
		    a[k] *= b[k];
		break;
	    case 'd':
		for(k = 0; k < leng; ++k)
		    a[k] /= b[k];
		break;
  	    case 'A':
	        for(k = 0; k < leng; ++k)
		    a[k] = atan2(b[k], a[k]);
		break;
	    default:
		break;
        }

	fwritef(a, sizeof(*a), leng, stdout);
    }
    return(0);
}

