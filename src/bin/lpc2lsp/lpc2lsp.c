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
*    Transform LPC to LSP                                               *
*                                                                       *
*                                        1998.11 K.Koishida             *
*                                                                       *
*       usage:                                                          *
*               lpc2lsp [ options ] [ infile ] > stdout                 *
*       options:                                                        *
*               -m m  :  order of LPC                     [25]          *
*               -s s  :  sampling frequency (kHz)         [10]          *
*               -k    :  output gain                      [TRUE]        *
*               -l    :  output log gain                  [FALSE]       *
*               -o o  :  output format (see stdout)       [0]           *
*               (level 2)                                               *
*               -n n  :  split number of unit circle      [128]         *
*               -p p  :  maximum number of interpolation  [4]           *
*               -d d  :  end condition of interpolation   [1e-6]        *
*      infile:                                                          *
*               LP coefficients                                         *
*                       , K, a(1), ..., a(m),                           *
*      stdout:                                                          *
*               output format LSP                                       *
*                       0  normalized frequency (0 ~ pi)                *
*                       1  normalized frequency (0 ~ 0.5)               *
*                       2  frequency (kHz)                              *
*                       3  frequency (Hz)                               *
*               LSP                                                     *
*                       , f(1), ..., f(m),                              *
*      require:                                                         *
*              lpc2lsp()                                                *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDER    25
#define SAMPLING 10
#define OTYPE    0
#define SPNUM    128
#define MAXITR   4
#define END      1e-6
#define GAIN     TR
#define LOGGAIN  FA

char *BOOL[] = {"FALSE", "TRUE"};
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
   fprintf(stderr, " %s - transform LPC to LSP\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of LPC                            [%d]\n", ORDER);
   fprintf(stderr, "       -s s  : sampling frequency                      [%d]\n", SAMPLING);
   fprintf(stderr, "       -k    : output gain                             [%s]\n", BOOL[GAIN]);
   fprintf(stderr, "       -l    : output log gain rather than linear gain [%s]\n", BOOL[GAIN]);
   fprintf(stderr, "       -o o  : output format                           [%d]\n", OTYPE);
   fprintf(stderr, "                 0 (normalized frequency [0...pi])\n");
   fprintf(stderr, "                 1 (normalized frequency [0...0.5])\n");
   fprintf(stderr, "                 2 (frequency [kHz])\n");
   fprintf(stderr, "                 3 (frequency [Hz])\n");
   fprintf(stderr, "     (level 2)\n");
   fprintf(stderr, "       -n n  : split number of unit circle             [%d]\n", SPNUM);
   fprintf(stderr, "       -p p  : maximum number of interpolation         [%d]\n", MAXITR);
   fprintf(stderr, "       -d d  : end condition of interpolation          [%g]\n", END);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       LP coefficients (%s)                         [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       LSP (%s)\n", FORMAT);
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
   int m=ORDER, otype=OTYPE, sampling=SAMPLING, n=SPNUM, p=MAXITR, i;
   FILE *fp=stdin;
   double *a, *lsp, end=END;
   Boolean gain=GAIN, loggain=LOGGAIN;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
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
         case 's':
            sampling = atoi(*++argv);
            --argc;
            break;
         case 'o':
            otype = atoi(*++argv);
            --argc;
            break;
         case 'p':
            p = atoi(*++argv);
            --argc;
            break;
         case 'n':
            n = atoi(*++argv);
            --argc;
            break;
         case 'd':
            end = atof(*++argv);
            --argc;
            break;
         case 'k':
            gain = 1 - gain;
            break;
         case 'l':
            loggain = 1 - loggain;
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

   lsp = dgetmem(m+m+1);
   a = lsp + m;

   while (freadf(a, sizeof(*a), m+1, fp)==m+1) {
      lpc2lsp(a, lsp, m, n, p, end);

      if (otype==0)
         for (i=0; i<m; i++)
            lsp[i] *= PI2;
      else if (otype==2 || otype==3)
         for (i=0; i<m; i++)
            lsp[i] *= sampling;
      if (otype==3)
         for (i=0; i<m; i++)
            lsp[i] *= 1000;
            
      if (gain) {
         if (loggain)
            *a = log(*a); 
         fwritef(a,sizeof(*a),1,stdout);
      }
      fwritef(lsp, sizeof(*lsp), m, stdout);
   }
   
   return(0);
}

