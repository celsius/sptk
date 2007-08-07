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
   $Id: SPTK.h,v 1.8 2007/08/07 07:17:20 heigazen Exp $ 
   
   Speech Signal Processing Toolkit
   SPTK.h
***********************************************************/

#ifndef PI
#define PI  3.14159265358979323846
#endif  /* PI */

#ifndef PI2
#define PI2 6.28318530717958647692
#endif  /* PI2 */

#ifndef M_PI
#define M_PI  3.1415926535897932384626433832795
#endif  /* M_PI */

#ifndef M_2PI
#define M_2PI 6.2831853071795864769252867665590
#endif  /* M_2PI */

#define LN_TO_LOG 4.3429448190325182765

/* #ifndef ABS(x) */
#define ABS(x) ((x<0.0) ? -x : x)
/* #endif */

/* enum for Boolean */
typedef enum _Boolean {FA, TR} Boolean;

/* enum for window type */
typedef enum _Window {
   BLACKMAN,
   HAMMING,
   HANNING,
   BARTLETT,
   TRAPEZOID,
   RECTANGULAR
} Window;

/* struct for complex */
typedef struct {
   double re;
   double im;
} complex;


/* library routines */
double agexp (double r, double x, double y);
int cholesky (double *c, double *a, double *b, const int n, double eps);
#ifdef DOUBLE
#define fwritef fwrite
#define freadf  fread
#else
int fwritef (double *ptr, const unsigned int size, const int nitems, FILE *fp);
int freadf (double *ptr, const unsigned int size, const int nitems, FILE *fp);
#endif /* DOUBLE */
void fillz (void *ptr, const int size, const int nitem);
FILE *getfp (char *name, char *opt);
short *sgetmem (const int leng);
long *lgetmem (const int leng);
double *dgetmem (const int leng);
float *fgetmem (const int leng);
/* real *rgetmem (const int leng); */
float **ffgetmem (const int leng);
char *getmem (const int leng, const unsigned int size);
double gexp (const double r, const double x);
double glog (const double r, const double x);
int ifftr (double *x, double *y, const int l);
void multim (double x[], const int xx, const int xy, double y[], const int yx, const int yy, double a[]);
void addm (double x[], double y[], const int xx, const int yy, double a[]);
void movem (void *a, void *b, const int size, const int nitem);
int mseq (void);
int theq (double *t, double *h, double *a, double *b, const int n, double eps);
int toeplitz (double *t, double *a, double *b, const int n, double eps);


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
double df2 (const double x, const double sf, const double f0p, const double wbp, const double f0z, const double wbz,
            const int fp, const int fz, double *buf, int *bufp);
double dfs (double x, double *a, int m, double *b, int n, double *buf, int *bufp);
int fft (double *x, double *y, const int m);
int checkm (const int m);
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
int ifft2 (double x[], double y[], const int n);
double iglsadf (double x, double *c, const int m, const int n, double *d);
double iglsadf1 (double x, double *c, const int m, const int n, double *d);
double gzerodf (double x, double *c, int m, const double g, double *d);
void ignorm (double *c1, double *c2, int m, const double g);
double imglsadf (double x, double *b, const int m, const double a, const int n, double *d);
double imglsadff (double x, double *b, const int m, const double a, double *d);
double imglsadf1 (double x, double *b, const int m, const double a, const int n, double *d);
double imglsadff1 (double x, double *b, const int m, const double a, const double g, double *d);
double imglsadft (double x, double *b, const int m, const double a, const int n, double *d);
double imglsadfft (double x, double *b, const int m, const double a, double *d);
double imglsadf1t (double x, double *b, const int m, const double a, const int n, double *d);
double imglsadff1t (double x, double *b, const int m, const double a, const double g, double *d);
void imsvq (int *index, double *cb, const int l, int *cbsize, const int stage, double *x);
void ivq (const int index, double *cb, const int l, double *x);
void lbg (double *x, const int l, const int tnum, double *icb, int icbsize, double *cb, const int ecbsize, const double delta, const double end);
int levdur (double *r, double *a, const int m, double eps);
double lmadf (double x, double *c, const int m, const int pd, double *d);
double lmadf1 (double x, double *c, const int m, double *d, const int m1, const int m2, const int pd);
double lmafir (double x, double *c, const int m, double *d, const int m1, const int m2);
int lpc (double *x, const int flng, double *a, const int m);
void lpc2c (double *a, int m1, double *c, const int m2);
int lpc2lsp (double *lpc, double *lsp, const int order, const int numsp, const int maxitr, const double eps);
double chebpoly (const double x, double *c, const int mh);
int lpc2par (double *a, double *k, const int m);
void lsp2lpc (double *lsp, double *a, const int m);
int lspcheck (double *lsp, const int ord);
void lsparrange (double *lsp, const int ord);
double lspdf_even (double x, double *f, const int m, double *d);
double lspdf_odd (double x, double *f, const int m, double *d);
double ltcdf (double x, double *k, int m, double *d);
void mc2b (double *mc, double *b, int m, const double a);
int mcep (double *xw, const int flng, double *mc, const int m, const double a, const int itr1, const int itr2, const double dd, const double e);
void frqtr (double *c1, int m1, double *c2, int m2, const double a);
void mgc2mgc (double *c1, const int m1, const double a1, const double g1, double *c2, const int m2, const double a2, const double g2);
void mgc2sp (double *mgc, const int m, const double a, const double g, double *x, double *y, const int flng);
int mgcep (double *xw, int flng, double *b, const int m, const double a, const double g, const int n, const int itr1, const int itr2, const double dd, const double e);
double newton (double *x, const int flng, double *c, const int m, const double a, const double g, const int n, const int j);
double gain(double *er, double *c, int m, double g);
void b2c(double *b, int m1, double *c, int m2, double a);
void ptrans(double *p, int m, double a);
void qtrans(double *q, int m, double a);
double mglsadf (double x, double *b, const int m, const double a, const int n, double *d);
double mglsadff (double x, double *b, const int m, const double a, double *d);
double mglsadf1 (double x, double *b, const int m, const double a, const int n, double *d);
double mglsadff1 (double x, double *b, const int m, const double a, const double g, double *d);
double mglsadft (double x, double *b, const int m, const double a, const int n, double *d);
double mglsadfft (double x, double *b, const int m, const double a, double *d);
double mglsadf1t (double x, double *b, const int m, const double a, const int n, double *d);
double mglsadff1t (double x, double *b, const int m, const double a, const double g, double *d);
double mlsadf (double x, double *b, const int m, const double a, const int pd, double *d);
double mlsadf1 (double x, double *b, const int m, const double a, const int pd, double *d);
double mlsadf2 (double x, double *b, const int m, const double a, const int pd, double *d);
double mlsafir (double x, double *b, const int m, const double a, double *d);
void msvq (double *x, double *cb, const int l, int *cbsize, const int stage, int *index);
void norm0 (double *x, double *y, int m);
int nrand (double *p, const int leng, const int seed);
double nrandom (unsigned long *next);
double rnd (unsigned long *next);
unsigned long srnd (const unsigned int seed);
void par2lpc (double *k, double *a, const int m);
void phase (double *p, const int mp, double *z, const int mz, double *ph, const int flng, const int unlap);
double pitch (double *xw, const int l, const double thresh, const int low, const int high,
              const double eps, const int m, const int itr1, const int itr2, const double end);
double poledf (double x, double *a, int m, double *d);
double poledft (double x, double *a, int m, double *d);
void reverse (double *x, const int l);
double rmse (double *x, double *y, const int n);
void output_root_pol (complex *x, int odr, int form);
complex *cplx_getmem (const int leng);
void root_pol (double *a, const int odr, complex *x, const int a_zero, const double eps, const int itrat);
int sp2mgc (double *xw, const int flng, double *b, const int m, const double a, const double g, const int n, 
            const int itr1, const int itr2, const double dd, const double e, const int itype);
int smcep (double *xw, const int flng, double *mc, const int m, const int fftsz, const double a,
           const double t, const int itr1, const int itr2, const double dd, const double e);
void frqt_a (double *al, const int m, const int fftsz, const double a, const double t);
void freqt2 (double *c1, const int m1, double *c2, const int m2, const int fftsz, const double a, const double t);
void ifreqt2(double *c1,int m1,double *c2,int m2,int fftsz,double a,double t);
void frqtr2 (double *c1,int m1,double *c2,int m2,int fftsz,double a,double t);
double warp (const double w, const double a, const double t);
double derivw (const double w, const double  a, const double t);
int uels (double *xw, const int flng, double *c, const int m, const int itr1, const int itr2, const double dd, const double e);
void lplp (double *r, double *c, const int m);
double ulaw_c (const double x, const double max, const double mu);
double ulaw_d (const double x, const double max, const double mu);
int vq (double *x, double *cb, const int l, const int cbsize);
double edist (double *x, double *y, const int m);
double window (Window type, double *x, const int size, const int nflg);
double *blackman (double *w, const int leng);
double *hamming (double *w, const int leng);
double *hanning (double *w, const int leng);
double *bartlett (double *w, const int leng);
double *trapezoid (double *w, const int leng);
double *rectangular (double *w, const int leng);
double zcross (double *x, const int fl, const int n);
double zerodf (double x, double *b, int m, double *d);
double zerodft (double x, double *b, const int m, double *d);
double zerodf1 (double x, double *b, int m, double *d);
double zerodf1t (double x, double *b, const int m, double *d);

