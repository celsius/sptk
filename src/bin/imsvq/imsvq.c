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
*    	Decoder of Multi Stage Vector Quantization			*
*									*
*					1996.1  K.Koishida		*
*									*
*	usage:								*
*		imsvq [options] [ infile ] > stdout 			*
*	options:							*
*		-l l   :  length of vector		[26]		*
*		-n n   :  order of vector		[25]		*
*		-s s f :  codebook  			[N/A N/A]	*
*			s: codebook size				*
*			f: codebook file				*
*	infile:								*
*		codebook index (int)					*
*		    index(0), index(1), ..., index(N),			*
*	stdout:								*
*		quantized vector					*
*		    x'(0), x'(1), ..., x'(l-1),				*
*	notice:								*
*		-s option are specified number of stages		*
*	require:							*
*		imsvq(), ivq()						*
*									*
************************************************************************/

static char *rcs_id = "$Id: imsvq.c,v 1.4 2006/12/15 11:06:44 mr_alex Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*  Default Values  */
#define LENG		26


/*  Command Name  */
char	*cmnd;


void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - decoder of multi stage vector quantization \n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "         -l l   : length of vector   [%d]\n", LENG);
    fprintf(stderr, "         -n n   : order of vector    [%d]\n", LENG-1);
    fprintf(stderr, "         -s s f : codebook           [N/A N/A]\n");
    fprintf(stderr, "                   s: codebook size\n");
    fprintf(stderr, "                   f: codebook file\n");
    fprintf(stderr, "         -h     : print this message\n");
    fprintf(stderr, "  stdin:\n");
    fprintf(stderr, "         index (int)\n");
    fprintf(stderr, "  stdout:\n");
    fprintf(stderr, "         quantized vectors (float)\n");
    fprintf(stderr, "  codebook file:\n");
    fprintf(stderr, "         codebook (float)\n");
    fprintf(stderr, "  notice:\n");
    fprintf(stderr, "         -s option are specified number of stages\n");
    fprintf(stderr, "\n");
    exit(status);
}


int main(int argc, char **argv)
{
    int		l = LENG, *cbsize, *index,
		stage = 0, ss = 0, num, i;
    FILE	*fp = stdin, *fpcb;
    double	*x, *cb = NULL, *p;
    char	**cbfile;

    cbsize = (int *)calloc(argc/2, sizeof(*cbsize));
    index = (int *)calloc(argc/2, sizeof(*index));
    cbfile = (char **)calloc(argc/2, sizeof(**cbfile));

    p = cb;
    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;

    while (--argc)
	if (**++argv == '-'){
	    switch (*(*argv+1)) {
		case 'l':
		    l = atoi(*++argv);
		    --argc;
		    break;
		case 'n':
		    l = atoi(*++argv)+1;
		    --argc;
		    break;
		case 's':
		    cbsize[stage] = atoi(*++argv);
		    cbfile[stage++] = *++argv;
		    argc -= 2;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	}
	else
	    fp = getfp(*argv,"r");
	
    for(i=0,num=0; i<stage; i++)
	num += cbsize[i];
    cb = dgetmem(num * l);
    p = cb;

    for(i=0; i<stage; i++){
	fpcb = getfp(cbfile[i], "r");
	if(freadf(p, sizeof(*p), cbsize[i]*l, fpcb) != cbsize[i]*l){
	    fprintf(stderr,"%s : Codebook size error of %d stage !\n",cmnd, ss);
	    exit(1);
	}
	p += cbsize[i] * l;
    }
	    
    x = dgetmem(l);

    while (fread(index, sizeof(*index), stage, fp) == stage){
	imsvq(index, cb, l, cbsize, stage, x);
	fwritef(x, sizeof(*x), l, stdout);
    }
    exit(0);
}

