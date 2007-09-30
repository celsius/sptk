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
*    GLSA Digital Filter for Speech Synthesis                           *
*                                                                       *
*                                       1996.3  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               glsadf [ options ] gcfile [ infile ] > stdout           *
*       options:                                                        *
*               -m m     :  order of generalized cepstrum   [25]        *
*               -g g     :  -1/gamma                        [1]         *
*               -p p     :  frame period                    [100]       *
*               -i i     :  interpolation period            [1]         *
*               -n       :  regard input as normalized      [FALSE]     *
*                           generalized cepstrum                        *
*               -k       :  filtering without gain          [FALSE]     *
*               -P Pa    :  order of Pade approximation     [4]         *
*        infile:                                                        *
*               generalized cepstral coefficients                       *
*                   , c(0), c(1), ..., c(M),                            *
*               excitation sequence                                     *
*                   , x(0), x(1), ...,                                  *
*        stdout:                                                        *
*               filtered sequence                                       *
*                   , y(0), y(1), ...,                                  *
*        note:                                                          *
*               if g==0, LMA filter is used, P should be 4 or 5         *
*        require:                                                       *
*               glsadf(), lmadf()                                       *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: glsadf.c,v 1.21 2007/09/30 16:20:42 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDER 25
#define STAGE 1
#define FPERIOD 100
#define IPERIOD 1
#define NORM FA
#define NGAIN FA
#define PADEORD 4

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - GLSA digital filter for speech synthesis\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] gcfile [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of generalized cepstrum [%d]\n", ORDER);
   fprintf(stderr, "       -g g  : -1 / gamma                    [%d]\n", STAGE);
   fprintf(stderr, "       -p p  : frame period                  [%d]\n", FPERIOD);
   fprintf(stderr, "       -i i  : interpolation period          [%d]\n", IPERIOD);
   fprintf(stderr, "       -n    : regard input as normalized\n");
   fprintf(stderr, "               generalized cepstrum          [%s]\n", BOOL[NORM]);
   fprintf(stderr, "       -k    : filtering without gain        [%s]\n", BOOL[NGAIN]);
   fprintf(stderr, "       -P P  : order of Pade approximation   [%d]\n", PADEORD);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       filter input (%s)                  [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       filter output (%s)\n", FORMAT);
   fprintf(stderr, "  gcfile:\n");
   fprintf(stderr, "       generalized cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       if g==0, LMA filter is used, P should be 4 or 5\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char **argv)
{
   int m=ORDER, fprd=FPERIOD, iprd=IPERIOD, stage=STAGE, i, j, pd=PADEORD;
   FILE *fp=stdin, *fpc=NULL;
   Boolean norm=NORM, ngain=NGAIN;
   double *c, *inc, *cc, *d, x, gamma;
    
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
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
         case 'g':
            stage = atoi(*++argv);
            --argc;
            break;
         case 'p':
            fprd = atoi(*++argv);
            --argc;
            break;
         case 'i':
            iprd = atoi(*++argv);
            --argc;
            break;
         case 'n':
            norm = 1 - norm;
            break;
         case 'k':
            ngain = 1 - ngain;
            break;
         case 'P':
            pd = atoi(*++argv);
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else if (fpc==NULL)
         fpc = getfp(*argv, "r");
      else
         fp = getfp(*argv, "r");

   if (fpc==NULL) {
      fprintf(stderr,"%s : Cannot open cepstrum file!\n",cmnd);
      return(1);
   }

   if (stage!=0) {  /* GLSA */
      gamma = -1 / (double)stage;
   }
   else {  /* LMA */
      if ((pd<4)||(pd>5)) {
         fprintf(stderr,"%s : Order of Pade approximation should be 4 or 5!\n",cmnd);
         return(1);
      }
   }
    
   c = (stage!=0) ? dgetmem(m+m+m+3+m*stage) 
                  : dgetmem(m+m+m+3+(m+1)*pd*2);
   cc  = c  + m + 1;
   inc = cc + m + 1;
   d   = inc+ m + 1;
    
   if (freadf(c, sizeof(*c), m+1, fpc)!=m+1) return(1);
   if (stage!=0) {
      if (!norm) gnorm(c, c, m, gamma);
      for (i=1; i<=m; i++)   
         c[i] *= gamma;
   }

   for (;;) {
      if (freadf(cc, sizeof(*cc), m+1, fpc) != m+1) return(0);
      if (stage!=0) {  /* GLSA */
         if(!norm) gnorm(cc, cc, m, gamma);
         for (i=1; i<=m; i++)
            cc[i] *= gamma;
      }
   
      for (i=0; i<=m; i++)
         inc[i] = (cc[i] - c[i])*iprd / fprd;

      for (j=fprd, i=(iprd+1)/2; j--;) {
         if (freadf(&x, sizeof(x), 1, fp)!=1) return(0);

         if (stage!=0) {  /* GLSA */
            if (!ngain) x *= c[0];
            x = glsadf(x, c, m, stage, d);
         }
         else {  /* LMA */
            if (!ngain) x *= exp(c[0]);
            x = lmadf(x, c, m, pd, d);
         }
      
         fwritef(&x, sizeof(x), 1, stdout);
            
         if (!--i) {
            for (i=0; i<=m; i++) c[i] += inc[i];
            i = iprd;
         }
      }

      movem(cc, c, sizeof(*cc), m+1);
   }
   
   return(0);
}

