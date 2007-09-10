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
*    Extract Frame from Data Sequence                                   *
*                                                                       *
*                                       1985.11 K.Tokuda                *
*                                       1996.4  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               frame [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -l l     :  frame length                [256]           *
*               -p p     :  frame period                [100]           *
*               -n       :  no center start point       [FALSE]         *
*               +type    :  data type                   [f]             *
*                               c (char)     s (short)                  *
*                               i (int)      l (long)                   *
*                               f (float)    d (double)                 *
*                                                                       *
*       infile:                                                         *
*               data sequence                                           *
*                   , x(0), x(1), ...,                                  *
*       stdout:                                                         *
*               frame sequence                                          *
*                   0, 0, ..., 0, x(0), x(1), ..., x(l/2),              *
*                   , x(t), x(t+1),       ...,       x(t+l-1),          *
*                   , x(2t), x(2t+1),     ....                          *
*              if -n specified                                          *
*                   x(0), x(1),           ...,       x(l),              *
*                   , x(t), x(t+1),       ...,       x(t+l-1),          *
*                   , x(2t), x(2t+1),     ....                          *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG 256
#define FPERIOD 100
#define NOCTR FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - extract frame from data sequence\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : frame length          [%d]\n", LENG);
   fprintf(stderr, "       -p p  : frame period          [%d]\n", FPERIOD);
   fprintf(stderr, "       -n    : no center start point [%s]\n", BOOL[NOCTR]);
   fprintf(stderr, "       +type : data type             [f]\n");
   fprintf(stderr, "                c (char)      s (short)\n");
   fprintf(stderr, "                i (int)       l (long)\n");
   fprintf(stderr, "                f (float)     d (double)\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                 [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       extracted data sequence\n");
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
   int l=LENG, fprd=FPERIOD, size=sizeof(float), ns, i;
   FILE *fp=stdin;
   Boolean noctr = NOCTR;
   char *x, *xx, *p1, *p2, *p;
   char *s, c;
    
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (*(s=*++argv)=='-') {
         c = *++s;
         switch (c) {
         case 'l':
            l = atoi(*++argv);
            --argc;
            break;
         case 'p':
            fprd = atoi(*++argv);
            --argc;
            break;
         case 'n':
            noctr = 1 - noctr;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else if (*s=='+') {
         c = *++s;
         switch (c) {
         case 'c':
            size = sizeof(char);
            break;
         case 's':
            size = sizeof(short);
            break;
         case 'l':
            size = sizeof(long);
            break;
         case 'i':
            size = sizeof(int);
            break;
         case 'f':
            size = sizeof(float);
            break;
         case 'd':
            size = sizeof(double);
            break;
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else
         fp = getfp(*argv, "r");

   x = (char *)dgetmem(size*l);

   if (!noctr) {
      i = (int)((l + 1) / 2);
      if (fread(&x[(int)(l/2)*size], size, i, fp)!=i) 
         return 0;
   }
   else {
      if (fread(x, size, l, fp)!=l)
      return 0;
   }

   fwrite(x, size, l, stdout);

   if ((ns = (l-fprd))>0) {
      p = &x[fprd * size];
      for (;;) {
         p1 = x;  p2 = p;
         i = ns * size;
         while (i--) *p1++ = *p2++;
   
         if (fread(p1, size, fprd, fp)!=fprd) break;
            fwrite(x, size, l, stdout);
      }
   }
   else {
      i = -ns;
      xx = (char *)dgetmem(i*size);
      for (;;) {
         if (fread(xx, size, i, fp)!=i) break;

         if (fread(x, size, l, fp) != l) break;
         fwrite(x, size, l, stdout);
      }
   }
   
   return 0;
}

