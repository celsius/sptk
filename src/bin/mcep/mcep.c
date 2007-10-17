/*
  ---------------------------------------------------------------  
            Speech Signal Processing Toolkit (SPTK)

                      SPTK Working Group                           
                                                                   
                  Department of Computer Science                   
                  Nagoya Institute of Technology                   
                               and                                 
   Interdisciplinary Graduate School of Science and Engineering    
                  Tokyo Institute of Technology                    
                                                                   
                     Copyright (c) 1984-2007                       
                       All Rights Reserved.                        
                                                                   
  Permission is hereby granted, free of charge, to use and         
  distribute this software and its documentation without           
  restriction, including without limitation the rights to use,     
  copy, modify, merge, publish, distribute, sublicense, and/or     
  sell copies of this work, and to permit persons to whom this     
  work is furnished to do so, subject to the following conditions: 
                                                                   
    1. The source code must retain the above copyright notice,     
       this list of conditions and the following disclaimer.       
                                                                   
    2. Any modifications to the source code must be clearly        
       marked as such.                                             
                                                                   
    3. Redistributions in binary form must reproduce the above     
       copyright notice, this list of conditions and the           
       following disclaimer in the documentation and/or other      
       materials provided with the distribution.  Otherwise, one   
       must contact the SPTK working group.                        
                                                                   
  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF TECHNOLOGY,   
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM   
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL       
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSTITUTE OF         
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE   
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY        
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,  
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTUOUS   
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR          
  PERFORMANCE OF THIS SOFTWARE.                                    
                                                                   
  ---------------------------------------------------------------  
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
*               -l l     :  frame length (FFT length)       [256]       *
*               -q q     :  Input format                    [0]         *
*                              0 (windowed data sequence)               *
*                              1 (20*log|f(w)|)                         *
*                              2 (ln|f(w)|)                             *
*                              3 (|f(w)|)                               *
*                              4 (|f(w)|^2)                             *
*               (level 2)                                               *
*               -i i     :  minimum iteration               [2]         *
*               -j j     :  maximum iteration               [30]        *
*               -d d     :  end condition                   [0.001]     *
*               -e e     :  small value added to periodgram [0]         *
*               -f f     :  mimimum value of the determinant            *
*                           of the normal matrix            [0.000001]  *
*       infile:                                                         *
*               windowed sequence (if q==0)                             *
*                       , x(0), x(1), ..., x(L-1),                      *
*               spectrum sequence (otherwise)                           *
*                       , f(0), f(1), ..., f(L/2),                      *
*       stdout:                                                         *
*               mel cepstrum                                            *
*                       , c~(0), c~(1), ..., c~(M),                     *
*       require:                                                        *
*               mcep()                                                  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: mcep.c,v 1.21 2007/10/17 17:16:03 heigazen Exp $";


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
#define ALPHA 0.35
#define ORDER 25
#define FLENG 256
#define ITYPE 0
#define MINITR 2
#define MAXITR 30
#define END 0.001
#define EPS 0.0
#define MINDET 0.000001

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
   fprintf(stderr, "       -a a  : all-pass constant                [%g]\n", ALPHA);
   fprintf(stderr, "       -m m  : order of mel cepstrum            [%d]\n", ORDER);
   fprintf(stderr, "       -l l  : frame length                     [%d]\n", FLENG);
   fprintf(stderr, "       -q q  : input format                     [%d]\n", ITYPE);
   fprintf(stderr, "                 0 (windowed sequence\n");
   fprintf(stderr, "                 1 (20*log|f(w)|)\n");
   fprintf(stderr, "                 2 (ln|f(w)|)\n");
   fprintf(stderr, "                 3 (|f(w)|)\n");
   fprintf(stderr, "                 4 (|f(w)|)^2\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "     (level 2)\n");
   fprintf(stderr, "       -i i  : minimum iteration                [%d]\n", MINITR);
   fprintf(stderr, "       -j j  : maximum iteration                [%d]\n", MAXITR);
   fprintf(stderr, "       -d d  : end condition                    [%g]\n", END);
   fprintf(stderr, "       -e e  : small value added to periodgram  [%g]\n", EPS);
   fprintf(stderr, "       -f f  : mimimum value of the determinant [%g]\n", MINDET);
   fprintf(stderr, "               of the normal matrix\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       windowed sequences (%s)              [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       mel-cepstrum (%s)\n", FORMAT);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   int m=ORDER, flng=FLENG, ilng=FLENG, itr1=MINITR, itr2=MAXITR, itype=ITYPE, flag=0;
   FILE *fp=stdin;
   double *mc, *x, a=ALPHA, end=END, e=EPS, f=MINDET;

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
         case 'q':
            itype = atoi(*++argv);
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
         case 'f':
            f = atof(*++argv);
            --argc;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "rb");

   if (itype==0)
      ilng = flng;
   else
      ilng = flng/2+1;
   
   x = dgetmem(flng+m+1);
   mc = x + flng;

   while (freadf(x, sizeof(*x), ilng, fp)==flng) {
      flag = mcep(x, flng, mc, m, a, itr1, itr2, end, e, f, itype);
      fwritef(mc, sizeof(*mc), m+1, stdout);
   }
   
   return(0);
}

