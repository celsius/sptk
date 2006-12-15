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
*    Find NaN & Infty & Indefinite Values				*
*									*
*					1997.7	G.Hirabayashi		*
*									*
*	usage:								*
*		nan [ options ] [ infile ] > stdout			*
*	options:							*
*		-d       :  input is assumed to be double       [FALSE]	*
*	infile:								*
*		stdin for default					*
*		input is assumed to be float				*
*									*
************************************************************************/
static char *rcs_id = "$Id: nan.c,v 1.5 2006/12/15 11:06:52 mr_alex Exp $";

/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>
#include <string.h>

/*  Required Functions  */
void nan_tmp(FILE *fp, int dflag);

/* Defualt Values */
#define DFLAG			0
#define F_MASKEXP		0x7F800000L
#define F_MASKSIG		0x007FFFFFL
#define F_INDEFINITE		0xFFC00000L
#define	D_MASKEXP		0x7FF00000L
#define	D_MASKSIG		0x000FFFFFL
#define D_INDEFINITE		0xFFF80000L


/* Command Name */
char	*cmnd;


void usage(status)
int status;
{
	fprintf(stderr, "\n");
	fprintf(stderr, " %s - find NaN and Infty & Indefinite values\n", cmnd);
	fprintf(stderr, "\n");
	fprintf(stderr, "  usage:\n");
	fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
	fprintf(stderr, "  options:\n");
	fprintf(stderr, "       -d    : input is assumed to be double [FALSE]\n");
	fprintf(stderr, "       -h    : print this message\n");
	fprintf(stderr, "  infile:\n");
	fprintf(stderr, "       data sequence (float)        [stdin]\n");
	fprintf(stderr, "  stdout:\n");
	fprintf(stderr, "       result messages\n");
	fprintf(stderr, "\n");
	exit(status);
}

int main(int argc,char *argv[])
{
	FILE	*fp;
	char	*s, *infile = NULL, c;
	int	dflag = DFLAG;
	
        if ((cmnd = strrchr(argv[0], '/')) == NULL)
		cmnd = argv[0];
        else
		cmnd++;
	while (--argc)
	{
	    if(*(s = *++argv) == '-')
	    {
		c = *++s;
		switch(c)
		{
		case 'd':
			dflag = 1 - dflag;
			break;
		case 'h':
			usage(0);
		default:
			fprintf(stderr, "%s: unknown option '%c'\n", cmnd, c);
			usage(1);
			break;
		}
	    }
	    else
		infile = s;
	}
	if(infile)	fp = getfp(infile, "r");
	else		fp = stdin;

	nan_tmp(fp, dflag);
	exit(0);
}

void nan_tmp(FILE *fp, int dflag)
{
	long	*upper, *lower, count = 0;
	double	xd;
	long	xf;
	
	if(dflag)
	{
	    while(fread(&xd, sizeof(xd), 1, fp))
	    {
		upper = (long *)&xd;
		lower = upper + 1;

		if(!((*upper & D_MASKEXP) ^ D_MASKEXP))
		{
		    if ((!(*upper & D_MASKSIG))&&(!*lower))
			fprintf(stdout, "[No. %ld] is Infty\n", count);
		    else if((*upper == D_INDEFINITE)&&(!*lower))
			fprintf(stderr, "[No. %ld] is Indifinite\n", count);
		    else
			fprintf(stdout, "[No. %ld] is NaN\n", count);
		}
		++count;
	    }
	}
	else
	{
	    while(fread(&xf, sizeof(xf), 1, fp))
	    {
		if(!((xf & F_MASKEXP) ^ F_MASKEXP))
		{
		    if(!(xf & F_MASKSIG))
			fprintf(stdout, "[No. %ld] is Infty\n", count);
		    else if(xf == F_INDEFINITE)
			fprintf(stderr, "[No. %ld] is Indifinite\n", count);
		    else
			fprintf(stdout, "[No. %ld] is NaN\n", count);
		}
		++count;
	    }
	}
}

