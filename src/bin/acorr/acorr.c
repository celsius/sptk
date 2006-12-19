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
*                                                                       *
*    Obtain Autocorrelation Sequence                                    *
*					1988.7  T.Kobayashi             *
*                                       1995.12 I.Naohiro   modified    *
*                                                                       *
*       usage:                                                          *
*               acorr [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -m m     :  order of sequence           [12]            *
*               -l l     :  frame length                [256]           *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be real                             *
*									*
************************************************************************/

static char *rcs_id = "$Id: acorr.c,v 1.4 2006/12/19 13:02:38 mr_alex Exp $";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>

/* Default Values */
#define LENG	256
#define ORDER	25


/* Command Name  */
char	*cmnd;


void usage(int status)
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - obtain autocorrelation sequence\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -m m  : order of sequence  [%d]\n", ORDER);
	fprintf(stderr, "       -l l  : frame length       [%d]\n", LENG);
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)      [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       autocorrelation sequence (float)\n");
#ifdef SPTK_VERSION
	fprintf(stderr, "\n");
	fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif		
	fprintf(stderr, "\n");
	exit(status);
}



int main(int argc, char **argv)
{
  	FILE	*fp = stdin;
	char	*s, *infile = NULL, c;
	double	*x, *r;
	int	l = LENG, np = ORDER;

	if ((cmnd = strrchr(argv[0], '/')) == NULL)
	    cmnd = argv[0];
        else
	    cmnd++;
	while (--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(*++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'm':
				np = atoi(s);
				break;
			case 'l':
				l = atoi(s);
				break;
			case 'h':
				usage(0);
			default:
				fprintf(stderr,
					"%s: unknown option '%c'\n", cmnd, c);
					usage(1);
				break;
			}
		}
		else
			infile = s;
	}
	if(infile)
		fp = getfp(infile,"r");

	x = dgetmem(l + np + 1);
	r = x + l;
	while(freadf(x, sizeof(*x), l, fp) == l) {
		acorr(x,l,r,np);
		fwritef(r, sizeof(*r), np + 1, stdout);
	}
	exit(0);	
}
