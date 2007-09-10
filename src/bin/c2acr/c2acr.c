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
*    Transform Cepstrum to Autocorrelation                              *
*                                                                       *
*                                       1986.9  K.Tokuda                *
*                                       1996.1  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               c2acr [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -m m     :  order of cepstrum      [25]                 *
*               -M M     :  order of autocorrelation   [25]             *
*               -l l     :  FFT length         [256]                    *
*       infile:                                                         *
*               cepstral coefficients                                   *
*                   , c(0), c(1), ..., c(M),                            *
*       stdout:                                                         *
*               autocorrelation coefficeints                            *
*                   , r(0), r(1), ..., r(N),                            *
*       require:                                                        *
*               c2acr()                                                 *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: c2acr.c,v 1.11 2007/09/10 12:49:31 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDERC 25
#define ORDERR 25
#define FLENG 256


/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - transform cepstrum to autocorrelation\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of cepstrum        [%d]\n", ORDERC);
   fprintf(stderr, "       -M M  : order of autocorrelation [%d]\n", ORDERR);
   fprintf(stderr, "       -l l  : FFT length               [%d]\n", FLENG);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       cepstrum (float)                 [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       autocorrelation (float)\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   int m=ORDERC, n=ORDERR, l=FLENG;
   FILE *fp = stdin;
   double *c, *r;
    
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'M':
            n = atoi(*++argv);
            --argc;
            break;
         case 'l':
            l = atoi(*++argv);
            --argc;
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

   c = dgetmem(m+n+2);
   r = c + m + 1;

   while (freadf(c, sizeof(*c), m+1, fp)==m+1) {
      c2acr(c, m, r, n, l);
      fwritef(r, sizeof(*r), n+1, stdout);
   }
   
   return(0);
}
