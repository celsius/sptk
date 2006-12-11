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
*    Extract Vector							*
*									*
*					1996.3  K.Koishida		*
*									*
*	usage:								*
*		extract [ options ] [ indexfile ] [ infile ] > stdout	*
*	options:							*
*		-l l     :  order of vector	[10]			*
*		-i i     :  codebook index	[0]			*
*	infile:								*
*		index file (int)					*
*		    , index(0), index(1), ..., 				*
*		data sequence						*
*		    , x(0), x(1), ...,					*
*	stdout:								*
*		vectors which belong to index c				*
*		    , x'(0), x'(1), ...,				*
*									*
************************************************************************/

static char *rcs_id = "$Id: extract.c,v 1.3 2006/12/11 07:16:36 mr_alex Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG		10
#define INDEX		0
#define SIZE		256


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - extract vector \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] ifile [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -l l  : order of vector    [%d]\n", LENG);
    fprintf(stderr, "       -i i  : codebook index     [%d]\n", INDEX);
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence (float)      [stdin]\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "       extracted vector (float)\n");
    fprintf(stderr, "  ifile:\n");
    fprintf(stderr, "       indexfile (int)\n");
    fprintf(stderr, "\n");
    exit(status);
}


int main(int argc, char **argv)
{
    int		l = LENG, index = INDEX, size = SIZE, i;
    FILE	*fp = stdin, *fpi = NULL;
    double	*x;
    
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc)
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'i':
		    index = atoi(*++argv);
		    --argc;
		    break;
		case 's':
		    size = atoi(*++argv);
		    --argc;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
        else if (fpi == NULL)
            fpi = getfp(*argv, "r");
        else
            fp = getfp(*argv, "r");

    x = dgetmem(l);

    while (freadf(x, sizeof(*x), l, fp) == l &&
	   fread(&i, sizeof(i), 1, fpi)== 1 )
	if (i == index)
	    fwritef(x, sizeof(*x), l, stdout);

    exit(0);
}

