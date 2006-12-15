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
*    Obtain Zero Cross							*
*									*
*					1996.3	N.Isshiki		*
*									*
*	usage:								*
*		zcross [ options ] [ infile ] >stdout			*
*	options:							*
*		-l l	 :  frame length		[256]		*
*		-n	 :  normalized by frame langth			*
*	infile:								*
*		stdin for default					*
*		input is assumed to be real				*
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>
#include <string.h>

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};

/* Default Values */
#define FLENG	256
#define NORM	FA

/* Command Name  */
char	*cmnd;

void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - zero cross\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -l l  : frame length               [%d]\n",FLENG);
	fprintf(stderr, "       -n    : normarized by frame length\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)              [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       zero cross rate (float)\n");
	fprintf(stderr, "\n");
	exit(status);
}
	
int main(int argc,char **argv)
{

  	FILE	*fp = stdin;
	char	*s, *infile = NULL, c;
	double	*x;
	int	flng = FLENG;
	double	z;
	Boolean norm = NORM;

        if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
        else
		cmnd++;
	
	while(--argc) {
		if(*(s = *++argv) == '-') {
			c = *++s;
			if(c != 'n' && *++s == '\0') {
				s = *++argv;
				--argc;
			}
			switch(c) {
			case 'l':
				flng = atoi(s);
				break;
			case 'n':
				norm = 1 - norm;
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
	
	x = dgetmem(flng);
	while(freadf(x, sizeof(*x), flng, fp) == flng){
		z = zcross(x, flng, norm);
		fwritef(&z, sizeof(z), 1, stdout);
	}
	exit(0);
}
