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

/***********************************************************
   Speech Signal Processing Toolkit
   SPTK.h
***********************************************************/

#ifndef PI
#define PI 3.14159265358979323846
#define PI2 6.28318530717958647692
#endif

/* #ifndef ABS(x) */
#define ABS(x) (x < 0. ? -x : x)
/* #endif */

typedef enum _Window {
   BLACKMAN,
   HAMMING,
   HANNING,
   BARTLETT,
   TRAPEZOID,
   RECTANGULAR
} Window;

void fillz(char *, int, int);
void movem(char *, char *, int, int);
short *sgetmem(int);
long *lgetmem(int);
float *fgetmem(int);
float **ffgetmem(int);
/*real   *rgetmem();*/
double *dgetmem(int);
char *getmem(int);

FILE *getfp(char *, char *);

#ifdef DOUBLE
#define fwritef fwrite
#define freadf  fread
#else /* DOUBLE */
int fwritef();
int freadf();
#endif /* DOUBLE */

int theq(double *, double *, double *, double *, int, double);

