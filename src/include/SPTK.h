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

#ifndef M_PI
#define M_PI  3.1415926535897932384626433832795
#define M_2PI 6.2831853071795864769252867665590
#endif

#define  LN_TO_LOG   4.3429448190325182765

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

typedef enum _Boolean {FA, TR} Boolean;
char *BOOL[] = {"FALSE", "TRUE"};


/* library routines */
double agexp (double, double, double);
int cholesky (double *, double *, double *, const int, double);
void fillz (void *, int, int);
FILE *getfp (char *, char *);
short *sgetmem (const int);
long *lgetmem (const int);
float *fgetmem (const int);
float **ffgetmem (const int);
/*real   *rgetmem();*/
double *dgetmem (const int);
char *getmem (const int, const unsigned int);
double gexp (const double, const double);
double glog (const double, const double);
int ifftr (double *, double *, const int);
void multim (double *, const int, const int, double *, const int, const int, double *);
void addm (double *, double *, const int, const int, double *);
void movem (void *, void *, const int, const int);
int mseq (void);
int theq (double *, double *, double *, double *, const int, double);
int toeplitz (double *, double *, double *, const int, double);
#ifdef DOUBLE
#define fwritef fwrite
#define freadf  fread
#else /* DOUBLE */
int fwritef (double *, const unsigned int, const int, FILE *);
int freadf  (double *, const unsigned int, const int, FILE *);
#endif /* DOUBLE */

/* tool routines */
double acep (double x, double *c, const int m, const double lambda, const double step, const double tau, const int pd, const double eps);
void acorr (double *x, int l, double *r, const int np);
double agcep (double x, double *c, const int m, const int stage, const double lambda, const double step, const double tau, const double eps);
double amcep (double x, double *b, const int m, const double a, const double lambda, const double step, const double tau, const int pd, const double eps);
void phidf (const double x, const int m, double a, double *d);
double average (double *x, const int n);
void vaverage (double *x, const int l, const int num, double *ave);
void b2mc (double *b, double *mc, int m, const double a);
void c2acr(double *c, const int m1, double *r, const int m2, const int flng);
void c2ir (double *c, const int nc, double *h, const int leng);
void ic2ir (double *h, const int leng, double *c, const int nc);
void c2sp (double *c, const int m, double *x, double *y, const int l);
void clip (double *x, const int l, const double min, const double max, double *y);
double df2 (const double x, const double sf, const double f0p, const double wbp, const double f0z, const double wbz, const int fp, const int fz, double *buf, int *bufp);
double dfs (double x, double *a, int m, double *b, int n, double *buf, int *bufp);
int fft (double *x, double *y, const int m);
int fft2 (double x[], double y[], const int n);
void fftcep (double *sp, const int flng, double *c, const int m, int itr, double ac);
int fftr (double *x, double *y, const int m);
int fftr2 (double x[], double y[], const int n);
void freqt (double *c1, const int m1, double *c2, const int m2, const double a);
void gc2gc (double *c1, const int m1, const double g1, double *c2, const int m2, const double g2);
int gcep (double *xw, const int flng, double *gc, const int m, const double g, const int itr1, const int itr2, const double d, const double e);
double glsadf (double x, double *c, const int m, const int n, double *d);
double glsadf1 (double x, double *c, const int m, const int n, double *d);
double gpoledf (double x, double *c, int m, const double g, double *d);
void gnorm (double *c1, double *c2, int m, const double g);
void grpdelay (double *x, double *gd, const int size, const int is_arma);
int histogram (double *x, const int size, const double min, const double max, const double step, double *h);
int ifft (double *x, double *y, const int m);

