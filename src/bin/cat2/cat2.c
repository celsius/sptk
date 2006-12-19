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
*    	concatenate and display files to the standard error 	        *
*									*
*					      2000.6  S.Sako	        *
*									*
*	usage:								*
*		cat2 [ options ] [ infile1 ] [ infile2 ] ...		*
*	options:							*
*		-n  :  output with line number                 [FALSE]	*
*									*
************************************************************************/

/*  Standard C Libraries **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  Command Name  */
char	*cmnd;

void usage(int status)
{
    fprintf(stderr, "\n");
    fprintf(stderr, " %s - concatenate and display files to the standard error\n",cmnd);
    fprintf(stderr, "\n");
    fprintf(stderr, "  usage:\n");
    fprintf(stderr, "       %s [ options ] [ infile1 ] [ infile2 ] ...\n", cmnd);
    fprintf(stderr, "  options:\n");
    fprintf(stderr, "       -n    : output with line number    [FALSE]\n");
    fprintf(stderr, "       -h    : print this message\n");
    fprintf(stderr, "  infile:\n");
    fprintf(stderr, "       data sequence                      [stdin]\n");
    fprintf(stderr, "  notice:\n");
#ifdef SPTK_VERSION
    fprintf(stderr, "\n");
    fprintf(stderr, " SPTK: version%.1f",SPTK_VERSION);
#endif
    fprintf(stderr, "\n");
    exit(status);
}

int main(int argc, char **argv)
{
    int  linenum = 0;
    int  stdinmode = 0;
    char buf[512];
    FILE *fp;

    if ((cmnd = strrchr(argv[0], '/')) == NULL)
	cmnd = argv[0];
    else
	cmnd++;
    while (--argc){
	if (**++argv == '-') {
	    switch (*(*argv+1)) {
		case 'n':
		    linenum = 1;
		    break;
		case 'h':
		    usage(0);
		default:
		    fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
		    usage(1);
		}
	} else {
		stdinmode = -1;
		if( (fp = fopen( *argv, "r")) == NULL){
			fprintf(stderr, "%s: cannot open %s\n", cmnd, *argv);
			exit(1);
		}
		while( fgets( buf, 512, fp) != 0){
			if( linenum > 0) fprintf( stderr, "%6d  ", linenum++);
			fputs( buf, stderr);
		}
	}
    }

    if( stdinmode == 0 ){
	    while( fgets( buf, 512, stdin) != 0){
		    if( linenum > 0) fprintf( stderr, "%6d  ", linenum++);
	    fputs( buf, stderr);
	    }
    }
    exit(0);
}
