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
/*                1996-2008  Nagoya Institute of Technology          */
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

/************************************************************************
*                                                                       *
*    DCT : Discrete Cosine Transformation                               *
*                                                                       *
*                                                                       *
*       usage:                                                          *
*               dct [ options ] [ infile ] > stdout                     *
*       options:                                                        *
*               -l l     :  DCT size                    [256]           *
*               -I       :  use complex number          [FALSE]         *
*       infile:                                                         *
*               stdin for default                                       *
*               input is assumed to be double                           *
*                                                                       *
************************************************************************/
static char *rcs_id =
    "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

static int size = 256, out = ' ';

/* Default Values */
#define SIZE 256

/*  Command Name  */
char *cmnd;

/* workspace */
static int dct_table_size = 0;
static float *dct_workspace = NULL;
static float *pLocalReal = NULL;
static float *pLocalImag = NULL;
static float *pWeightReal = NULL;
static float *pWeightImag = NULL;

#include <memory.h>

int usage(void)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s -dct: Discrete Cosine Transformation  \n",
           cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n",
           cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : DCT size             [%d]\n", SIZE);
   fprintf(stderr, "       -I    : use comlex number       [FALSE]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)        [stdin]\n",
           FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       DCT sequence (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(1);
}

int dft(float *pReal, float *pImag, const int nDFTLength)
{
   float *pTempReal;
   float *pTempImag;
   int k, n;
   double dTempReal, dTempImag;

   pTempReal = (float *) malloc(sizeof(float) * nDFTLength);
   pTempImag = (float *) malloc(sizeof(float) * nDFTLength);

   memcpy(pTempReal, pReal, sizeof(float) * nDFTLength);
   memcpy(pTempImag, pImag, sizeof(float) * nDFTLength);

   for (k = 0; k < nDFTLength; k++) {
      dTempReal = 0;
      dTempImag = 0;
      for (n = 0; n < nDFTLength; n++) {
         dTempReal += pTempReal[n] *
             cos(2.0 * PI * n * k / (double) nDFTLength) +
             pTempImag[n] *
             sin(2.0 * PI * n * k / (double) nDFTLength);
         dTempImag +=
             -pTempReal[n] *
             sin(2.0 * PI * n * k / (double) nDFTLength) +
             pTempImag[n] *
             cos(2.0 * PI * n * k / (double) nDFTLength);
      }
      pReal[k] = dTempReal;
      pImag[k] = dTempImag;

   }
   free(pTempReal);
   free(pTempImag);
}

/* nSize <= 0 : release resources  */
/*       >  0 : create cosine table of which size is 'nSize' */

static int dct_create_table(const int nSize)
{
   register int k, n;

   if (nSize == dct_table_size) {
      /* no needs to resize workspace. */
      return (0);
   } else {
      /* release resources to resize workspace. */
      if (dct_workspace != NULL) {
         free(dct_workspace);
         dct_workspace = NULL;
      }
      pLocalReal = NULL;
      pLocalImag = NULL;
      pWeightReal = NULL;
      pWeightImag = NULL;
   }

   /* getting resources. */
   if (nSize <= 0) {
      dct_table_size = 0;
      return (0);
   } else {
      dct_table_size = nSize;
      dct_workspace =
          (float *) malloc(sizeof(float) * (dct_table_size * 6));
      pWeightReal = dct_workspace;
      pWeightImag = dct_workspace + dct_table_size;
      pLocalReal = dct_workspace + (2 * dct_table_size);
      pLocalImag = dct_workspace + (4 * dct_table_size);

      for (k = 0; k < dct_table_size; k++) {
         pWeightReal[k] =
             cos(k * PI / (2.0 * dct_table_size)) /
             sqrt(2.0 * dct_table_size);
         pWeightImag[k] =
             -sin(k * PI / (2.0 * dct_table_size)) /
             sqrt(2.0 * dct_table_size);
      }
      pWeightReal[0] /= sqrt(2.0);
      pWeightImag[0] /= sqrt(2.0);
   }
}

int dct_based_on_dft(float *pReal, float *pImag, const float *pInReal,
                     const float *pInImag)
{
   register int n, k;

   for (n = 0; n < dct_table_size; n++) {
      pLocalReal[n] = pInReal[n];
      pLocalImag[n] = pInImag[n];
      pLocalReal[dct_table_size + n] = pInReal[dct_table_size - 1 - n];
      pLocalImag[dct_table_size + n] = pInImag[dct_table_size - 1 - n];
   }

   dft(pLocalReal, pLocalImag, dct_table_size * 2);

   for (k = 0; k < dct_table_size; k++) {
      pReal[k] =
          pLocalReal[k] * pWeightReal[k] -
          pLocalImag[k] * pWeightImag[k];
      pImag[k] =
          pLocalReal[k] * pWeightImag[k] +
          pLocalImag[k] * pWeightReal[k];
   }
}


int main(int argc, char *argv[])
{
   FILE *fp;
   char *s, *infile = NULL, c;
   int dft(float *pReal, float *pImag, const int nDFTLength);
   static int dct_create_table(const int nSize);
   int dct_based_on_dft(float *pReal, float *pImag, const float *pInReal,
			const float *pInImag);

   float *pReal, *pImag;
   int k, n;

   FILE *getfp();

   double *x, *y, *dgetmem();
   int size2;
   float *x2, *y2;

   if ((cmnd = strrchr(argv[0], '/')) == NULL)
      cmnd = argv[0];
   else
      cmnd++;

   while (--argc) {
      if (*(s = *++argv) == '-') {
         c = *++s;
         if ((c == 'l') && (*++s == '\0')) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            size = atoi(s);
            break;
         case 'I':
            out = c;
            break;
         case 'h':
         default:
            usage();
         }
      } else
         infile = s;
   }
   if (infile) 
     fp = getfp(infile, "rb");
   else fp = stdin;

   x = dgetmem(size2 = size + size);
   y = x + size;
   if ((pReal = (float *) calloc(size2, sizeof(float))) == NULL) {
     fprintf(stderr, "No memory allocated : pReal\n");
     exit(1);
   }
   pImag = pReal + size;
   if ((x2 = (float *) calloc(size2, sizeof(float))) == NULL) {
     fprintf(stderr, "No memory allocated : x2\n");
     exit(1);
   }
   y2 = x2 + size;
   
   while (!feof(fp)) {
     fillz(x, size2, sizeof(double));
     fillz(y, size, sizeof(double));
     if (freadf(x, sizeof(*x), size, fp) == 0)
       break;
     if(out=='I') {
       if (freadf(y, sizeof(*y), size, fp) == 0)
	 break;
     }
       
     for (k = 0; k < size; k++) {
       x2[k] = (float) x[k];
       y2[k] = (float) y[k];
     }

         
     dct_create_table(size);
     dct_based_on_dft(pReal, pImag, (const float *) x2,
		      (const float *) y2);
     
     fwrite(pReal, sizeof(*pReal), size, stdout);
     if(out=='I')
       fwrite(pImag, sizeof(*pReal), size, stdout);
     

   }

   if (infile) fclose(fp);

   return (0);
}
