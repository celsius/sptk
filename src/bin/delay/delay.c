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
*    Delay Sequence							*
*									*
*					1996.4  K.Koishida		*
*									*
*	usage:								*
*		delay [ options ] [ infile ] > stdout			*
*	options:							*
*		-s s     :  start sample		[0]		*
*		-f	 :  keep file size				*
*	infile:								*
*		data sequence						*
*		    , x(0), x(1), ...					*
*	stdout:								*
*		delayed data						*
*		    , 0, 0, ..., 0, x(1), x(2), ..., 			*
*		      ------------					*
*			s sample					*
*									*
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define START 0
#define KEEP FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - delay sequence\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -s s  : start sample      [%d]\n", START);
   fprintf(stderr, "       -f    : keep file length  [%s]\n", BOOL[KEEP]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (float)     [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       delayed data sequence (float)\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   int  start=START, leng=16384, i;
   FILE *fp=stdin;
   double *x;
   Boolean keep=KEEP;
   
   
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 's':
            start = atoi(*++argv);
            --argc;
            break;
         case 'f':
            keep = 1 - keep;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else 
         fp = getfp(*argv, "r");

   if (start>leng) {
      fprintf(stderr, "%s : Start point %d should be %d or less!\n", cmnd, start, leng);
      exit(1);
   }
            
   x = dgetmem(leng);
   while ((i = freadf(x+start, sizeof(*x), leng-start, fp))==leng-start) {
      fwritef(x, sizeof(*x), leng-start, stdout);
      movem(x+leng-start,x,sizeof(*x),start);
   }
    
   if (keep) fwritef(x,sizeof(*x),i,stdout);
   else fwritef(x, sizeof(*x), i+start, stdout);

   return(0);
}

