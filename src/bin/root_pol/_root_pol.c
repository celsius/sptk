/************************************************************************
*			                                     		*
*    Durand-Kerner-Aberth for Higher Order Equation			*
*									*
*					1988.12  K.Fukushi		*
*					1989.3  T.Kobayashi modified	*
*					1999.12	T.Yoshimura modified	*
*									*
*    Usage: root_pol(a, odr, x, a_zero, eps, itrat)                     *
*                                                                       *
*    Description:                                                       *
*                                                                       *
*        real*         a[2*odr];                                        *
*        int           odr, a_zero, itrat;                              *
*        double        eps;                                             *
*                                                                       *
*      Pn(x) = a[0]x^n + a[1]x^(n-1) + a[2]x^(n-2) + ... + a[n]         *
*                                                                       *
*    Output:                                                            *
*                                                                       *
*      x[0].re,x[0].im,...,x[n-1].re,x[n-1].im                          *
*                                                                       *
*                                                                       *
************************************************************************/

/*  Standard C Libraries  */
#include <stdio.h>
#include <math.h>
#include <SPTK.h>


typedef enum { plus, minus, multiply, divide } opt;
typedef struct { double re, im;} complex;


complex *cplx_getmem(leng)
    int leng;
{
    int i;
    complex *p = NULL;

    if ((p = (complex *)malloc(sizeof(complex)*leng)) == NULL){
	fprintf(stderr, "Memory allocation error !\n");
	exit(3);
    }

    for(i = 0;i < leng;i++) p[i].re = p[i].im = 0;

    return p;
}

double rad_root(x, i)
     double x;
     int i;
{
    if(x == 0.0)
        return -1.0;
    else
        return exp(log(x)/i);
}

complex c_math(c1, op, c2)
     complex c1, c2;
     opt op;
{
    double p;
    complex t;

    switch(op){
    case  plus :	
        t.re = c1.re + c2.re;
        t.im = c1.im + c2.im;
        break;
    case  minus :
        t.re = c1.re - c2.re;
        t.im = c1.im - c2.im;
        break;
    case  multiply :
        t.re = c1.re*c2.re - c1.im*c2.im;
        t.im = c1.re*c2.im + c1.im*c2.re;
        break;
    case  divide :
        p = c2.re*c2.re + c2.im*c2.im;
        t.re = (c1.re*c2.re + c1.im*c2.im)/p;
        t.im = (c1.im*c2.re - c1.re*c2.im)/p;
        break;
    default :
        t.re = c1.re;
        t.im = c1.im;
        break;
    }
    return t;
}

double c_mag(x)
     complex x;
{
    return sqrt(x.re*x.re + x.im*x.im);
}

double c_arg(x)
     complex x;
{
    return atan2(x.im, x.re);
}

void out(x, odr, form)
     complex *x;
     int odr, form;
{
    int 	i, k;
    double	mag, arg, *a;

    a = dgetmem(2*odr);

    switch (form) {
    case 1:
        for (k = i = 0; i < odr; i++) {
            a[k++] = c_mag(x[i+1]);
            a[k++] = c_arg(x[i+1]);
        }
        break;
    case 2:
    case 3:
        for (k = i = 0; i < odr; i++) {
            mag = 1 / c_mag(x[i+1]);
            arg = - c_arg(x[i+1]);
            if (form == 3) {
                a[k++] = mag;
                a[k++] = arg;
            } else {	
                a[k++] = mag * cos(arg);
                a[k++] = mag * sin(arg);
            }
        }
        break;
    case 0:		
    default:
        for (k = i = 0; i < odr; i++) {
            a[k++] = x[i+1].re;
            a[k++] = x[i+1].im;
        }
        break;
    }

    fwritef(a, sizeof(*a), odr*2, stdout);
}

void root_pol(a, odr, x, a_zero, eps, itrat)
     double	*a, eps;
     int	odr, a_zero, itrat;
     complex	*x;
{
    int 	i, j, k, l;
    double	th, th1, th2, cm, cmax;
    complex	cden, cnum, c1, *deltx;

    deltx = cplx_getmem(odr);

    if(!a_zero)
        for(i = 1; i <= odr; i++) a[i] /= a[0];

    cmax = 0;
    for (i = 2; i <= odr; i++) {
        cm = odr * rad_root(fabs(a[i]),i);
        if(cm > cmax) cmax = cm;
    }

    th1 = PI*2.0 / odr;
    th2 = th1 / 4.0;
    for(i = 1; i <= odr; i++){
        th = th1*(i-1) + th2;
        x[i].re = cmax*cos(th);
        x[i].im = cmax*sin(th);
    }

    l = 1;
    do {
        for(i = 1; i <= odr; i++){
            cden.re = 1.0;
            cden.im = 0.0;
            cnum.re = 1.0;
            cnum.im = 0.0;
            c1 = x[i];
            for(j = 1; j <= odr; j++){
                cnum = c_math(cnum, multiply, c1);
                cnum.re += a[j];
                if( j != i )
                    cden = c_math(cden, multiply, c_math(c1, minus, x[j]));
            }
            deltx[i] = c_math(cnum, divide, cden);
            x[i] = c_math(c1, minus, deltx[i]);
        }
        k = 1;
        while(k <= odr && c_mag(deltx[k++]) <= eps);
        l++;
    } while(l <= itrat && k <= odr);

    if(l > itrat){
        fprintf(stderr, "root_pol : No convergence.\n");
        exit(1);
    }
}
