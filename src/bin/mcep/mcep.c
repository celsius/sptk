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
*    Mel Cepstral Analysis                                              *
*                                                                       *
*                                     1993.5  K.Tokuda                  *
*                                     1996.1  K.Koishida                *
*                                                                       *
*       usage:                                                          *
*                mcep [ options ] [infile] > stdout                     *
*       options:                                                        *
*               -a alpha :  all-pass constant               [0.35]      *
*               -m m     :  order of mel cepstrum           [25]        *
*               -l l     :  frame length                    [256        *
*               (level 2)                                               *
*               -i i     :  minimum iteration               [2]         *
*               -j j     :  maximum iteration               [30]        *
*               -d d     :  end condition                   [0.001]     *
*               -e e     :  small value added to periodgram [0.0]       *
*       infile:                                                         *
*               data sequence                                           *
*                       , x(0), x(1), ..., x(L-1),                      *
*       stdout:                                                         *
*               mel cepstrum                                            *
*                       , c~(0), c~(1), ..., c~(M),                     *
*       require:                                                        *
*               mcep()                                                  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>

/*  Default Values  */
#define ALPHA 0.35
#define ORDER 25
#define FLENG 256
#define MINITR 2
#define MAXITR 30
#define END 0.001
#define EPS 0.0

#ifdef DOUBLE
char *FORMAT = "double";
#else
char *FORMAT = "float";
#endif /* DOUBLE */

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - mel cepstral analysis\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -a a  : all-pass constant               [%g]\n", ALPHA);
   fprintf(stderr, "       -m m  : order of mel cepstrum           [%d]\n", ORDER);
   fprintf(stderr, "       -l l  : frame length                    [%d]\n", FLENG);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "     (level 2)\n");
   fprintf(stderr, "       -i i  : minimum iteration               [%d]\n", MINITR);
   fprintf(stderr, "       -j j  : maximum iteration               [%d]\n", MAXITR);
   fprintf(stderr, "       -d d  : end condition                   [%g]\n", END);
   fprintf(stderr, "       -e e  : small value added to periodgram [%g]\n",EPS);
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       windowed sequences (%s)              [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       mel-cepstrum (%s)\n", FORMAT);
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
   int m=ORDER, flng=FLENG, itr1=MINITR, itr2=MAXITR, flag=0;
   FILE *fp=stdin;
   double *mc, *x, a=ALPHA, end=END, e=EPS;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'a':
            a = atof(*++argv);
            --argc;
            break;
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'l':
            flng = atoi(*++argv);
            --argc;
            break;
         case 'i':
            itr1 = atoi(*++argv);
            --argc;
            break;
         case 'j':
            itr2 = atoi(*++argv);
            --argc;
            break;
         case 'd':
            end = atof(*++argv);
            --argc;
            break;
         case 'e':
            e = atof(*++argv);
            --argc;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "r");

   x = dgetmem(flng+m+1);
   mc = x + flng;

   while (freadf(x, sizeof(*x), flng, fp)==flng) {
      flag = mcep(x, flng, mc, m, a, itr1, itr2, end, e);
      fwritef(mc, sizeof(*mc), m+1, stdout);
   }
   
   return(0);
}

