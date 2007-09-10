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
*    Generate Ramp Sequence                                             *
*                                                                       *
*                                          1996.4  K.Koishida           *
*                                                                       * 
*       usage:                                                          *
*               ramp [options] > stdout                                 *
*       options:                                                        *
*               -l l     :  length         [256]                        *
*               -n n  :  order             [l-1]                        *
*               -s s     :  start value    [0.0]                        *
*               -e e     :  end  value     [N/A]                        *
*               -t t     :  step size      [1.0]                        *
*       stdout:                                                         *
*               ramp sequence                                           *
*                       , s, s+t, s+2t, ..., s+(l-1)t                   *
*       notice:                                                         *
*               If l<0, generate infinite sequence                      *
*               When both -e and -l and -n are specified 2 or more,     *
*               latter argument is adopted.                             *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG  256
#define START  0.0
#define STEP  1.0
#define EFLAG FA


/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - generate ramp sequence\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : length                [%d]\n", LENG);
   fprintf(stderr, "       -n n  : order                 [l-1]\n");
   fprintf(stderr, "       -s s  : start value           [%g]\n", START);
   fprintf(stderr, "       -e e  : end value             [N/A]\n");
   fprintf(stderr, "       -t t  : step size             [%g]\n", STEP);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       ramp sequence (float)\n");
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       if l<0, generate infinite sequence\n");
   fprintf(stderr, "       When -l and -n and -e are specified 2 or more,\n");
   fprintf(stderr, "       latter argument is adopted.\n");
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
   int l=LENG, i;
   double start=START, step=STEP, end=START, x;
   Boolean eflag=EFLAG;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'l':
            l = atoi(*++argv);
            --argc;
            eflag = FA;
            break;
         case 'n':
            l = atoi(*++argv)+1;
            --argc;
            eflag = FA;
            break;
         case 's':
            start = atof(*++argv);
            --argc;
            break;
         case 'e':
            end = atoi(*++argv);
            --argc;
            eflag = TR;
            break;
         case 't':
            step = atof(*++argv);
            --argc;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }

   x = start;
   if (eflag)
      l = (end - start) / step + 1;

   for (i=0;; i++) {
      fwritef(&x, sizeof(x), 1, stdout);
      x += step;

      if (i==l-1) break;
   }

   return(0);
}

