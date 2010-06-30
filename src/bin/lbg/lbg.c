/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2010  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/******************************************************************************
*                                                                             *
*     LBG Algorithm for Vector Quantizer Design                               *
*                                                                             *
*                                             1996. 4  K.Koishida             *
*                                             2010. 6  A.Tamamori             *
*                                                                             *
*        usage:                                                               *
*                lbg [ options ] [ indexfile ] < stdin > stdout               *
*        options:                                                             *
*                -l l      :  length of vector                  [26]          *
*                -n n      :  order of vector                   [25]          *
*                -t t      :  number of training vector         [N/A]         *
*                -s s      :  initial codebook size             [1]           *
*                -e e      :  final codebook size               [256]         *
*                -f f      :  initial codebook filename         [NULL]        *
*                -m m      :  minimum num. of training          [NULL]        *
*                             vectors for each cell             [1]           *
*                -S sfile  :  seq. of seed for each centroid    [NULL]        *
*                -k k      :  type of split if the # of         [1]           *
*                             training vector is less than m                  *
*                             k = 1 : split the centroid                      *
*                                     with most train. vector                 *
*                             k = 2 : split the parent centroid               *
*                (level 2)                                                    *
*                -d d      :  end condition                     [0.0001]      *
*                -r r      :  splitting factor                  [0.0001]      *
*       infile:                                                               *
*                training vector (stdin)                                      *
*                        x(0), x(1), ... x(t*l-1)                             *
*       stdout:                                                               *
*               trained codebook                                              *
*                        cb'(0), cb'(1), ... cb(l*e-1)                        *
*       ifile: (if s>1)                                                       *
*              initial codebook (cbfile)                                      *
*                        cb(0), cb(1), ... cb(l*s-1)                          *
*       indexfile:                                                            *
*              VQ index of training vector (int)                              *
*                        index(0), index(1), ... index(t-1)                   *
*       sfile:                                                                *
*              seq. of seed given to each centroid (int)                      *
*                        seed(0), seed(1), ... , seed(e-1)                    *
*       notice:                                                               *
*              codebook size (s and e) must be power of 2                     *
*              -t option can be omitted, when input from redirect             *
*      require:                                                               *
*              lbg(), vq()                                                    *
*                                                                             *
******************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <strings.h>
#  ifndef HAVE_STRRCHR
#     define strrchr rindex
#  endif
#endif


#if defined(WIN32)
#  include "SPTK.h"
#else
#  include <SPTK.h>
#endif

/*  Default Values  */
#define LENG 26
#define TNUMBER -1
#define ICBSIZE 1
#define ECBSIZE 256
#define DELTA 0.0001
#define END 0.0001
#define MINTRAIN 1
#define SPLITTYPE 1

#define MAXVALUE 1e23
#define abs(x)  ( (x<0) ? (-(x)) : (x) )

/*  Command Name  */
char *cmnd;


void usage(int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - LBG algorithm for vector quantizer design \n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ ifile ]<stdin>stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l      : length of vector                   [%d]\n", LENG);
   fprintf(stderr, "       -n n      : order of vector                    [%d]\n", LENG - 1);
   fprintf(stderr, "       -t t      : number of training vector          [N/A]\n");
   fprintf(stderr, "       -s s      : initial codebook size              [%d]\n", ICBSIZE);
   fprintf(stderr, "       -e e      : final codebook size                [%d]\n", ECBSIZE);
   fprintf(stderr, "       -f f      : initial codebook filename          [NULL]\n");
   fprintf(stderr, "       -m m      : minimum number of training\n");
   fprintf(stderr, "                   vectors for each cell              [%d]\n", MINTRAIN);
   fprintf(stderr, "       -S sfile  : seq. of seed for each centroid     [NULL]\n");
   fprintf(stderr, "       -k k      : type of split if the number of     [%d]\n", SPLITTYPE);
   fprintf(stderr, "                   training vector is less than m\n");
   fprintf(stderr, "                   k = 1 : split the centroid\n");
   fprintf(stderr, "                           with most training vector\n");
   fprintf(stderr, "                   k = 2 : split the parent centroid\n");
   fprintf(stderr, "       -h        : print this message\n");
   fprintf(stderr, "     (level 2)\n");
   fprintf(stderr, "       -d d  : end condition             [%g]\n", END);
   fprintf(stderr, "       -r r  : splitting factor          [%g]\n", DELTA);
   fprintf(stderr, "  stdin:\n");
   fprintf(stderr, "       data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       codebook (%s)\n", FORMAT);
   fprintf(stderr, "  ifile:\n");
   fprintf(stderr, "       index (int)\n");
   fprintf(stderr, "  sfile:\n");
   fprintf(stderr, "       seed (int)\n");
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       codebook size (s and e) must be power of 2\n");
   fprintf(stderr,
           "       -t option can be omitted, when input from redirect\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main(int argc, char **argv)
{
   int l = LENG, icbsize = ICBSIZE, ecbsize = ECBSIZE, tnum = TNUMBER,
       ispipe, xsize, csize, i, j, *tindex, mintnum = MINTRAIN, *seed, split = SPLITTYPE;
   FILE *fp = stdin, *fpi = NULL, *fpcb = NULL, *fseed = NULL;
   double delta = DELTA, minerr = END, *x, *cb, *icb;
   double *p;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv == '-') {
         switch (*(*argv + 1)) {
         case 'l':
            l = atoi(*++argv);
            --argc;
            break;
         case 'n':
            l = atoi(*++argv) + 1;
            --argc;
            break;
         case 't':
            tnum = atoi(*++argv);
            --argc;
            break;
         case 's':
            icbsize = atoi(*++argv);
            --argc;
            break;
         case 'e':
            ecbsize = atoi(*++argv);
            --argc;
            break;
         case 'd':
            minerr = atof(*++argv);
            --argc;
            break;
         case 'r':
            delta = atof(*++argv);
            --argc;
            break;
         case 'f':
            fpcb = getfp(*++argv, "rb");
            --argc;
            break;
         case 'm':
             mintnum = atoi(*++argv);
             --argc;
             break;
         case 'S':
             fseed = getfp(*++argv, "rb");
             --argc;
             break;
         case 'k':
             split = atoi(*++argv);
             --argc;
             break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv + 1));
            usage(1);
         }
      } else
         fpi = getfp(*argv, "wb");

   if (tnum == -1) {
      ispipe = fseek(fp, 0L, 2);

#ifdef DOUBLE
      tnum = ftell(fp) / l / sizeof(double);
#else
      tnum = ftell(fp) / l / sizeof(float);
#endif                          /* DOUBLE */

      rewind(fp);
      if (ispipe == -1) {
         fprintf(stderr,
                 "%s : -t option must be specified, when input via pipe!\n",
                 cmnd);
         usage(1);
      }
   }

   xsize = tnum * l;
   csize = ecbsize * l;

   x = dgetmem(xsize);
   cb = dgetmem(csize);
   seed = (int *) dgetmem(ecbsize);

   if (freadf(x, sizeof(*x), xsize, fp) != xsize) {
      fprintf(stderr, "%s : Size error of training data!\n", cmnd);
      return (1);
   }

   if (fseed != NULL) {
      if(fread(seed, sizeof(*seed), ecbsize, fseed) != ecbsize) {
         fprintf(stderr, "%s : Size error of training data!\n", cmnd);
         return (1);
      }
   } else {
      for (i = 0; i < ecbsize; i++)
         seed[i] = i;
   }

   if (icbsize == 1) {
      icb = dgetmem(l);
      fillz(icb, sizeof(*icb), l);
      for (i = 0, p = x; i < tnum; i++)
         for (j = 0; j < l; j++)
            icb[j] += *p++;

      for (j = 0; j < l; j++)
         icb[j] /= (double) tnum;
   } else {
      icb = dgetmem(icbsize * l);
      if (freadf(icb, sizeof(*icb), icbsize * l, fpcb) != icbsize * l) {
         fprintf(stderr, "%s : Size error of initial codebook!\n", cmnd);
         return (1);
      }
   }

   lbg(x, l, tnum, icb, icbsize, cb, ecbsize, mintnum, seed, split, delta, minerr);

   fwritef(cb, sizeof(*cb), csize, stdout);

   if (fpi != NULL) {
      tindex = (int *) dgetmem(tnum);
      for (i = 0, p = x; i < tnum; i++, p += l)
         tindex[i] = vq(p, cb, l, ecbsize);

      fwritex(tindex, sizeof(*tindex), tnum, fpi);
   }

   return (0);
}
