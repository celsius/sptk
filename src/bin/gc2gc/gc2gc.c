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
*    Generalized Cepstral Transformation                                *
*                                                                       *
*                                       1987.11 K.Tokuda                *
*                                       1996.1  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               gc2gc [ options ] [ infile ] > stdout                   *
*       options:                                                        *
*               -m m   :  order of generalized cepstrum (input)   [25]  *
*               -g g   :  gamma of generalized cepstrum (input)   [0.0] *
*               -n     :  regard input as normalized cepstrum           *
*               -u     :  regard input as multiplied by gamma           *
*               -M M   :  order of generalized cepstrum (output)  [25]  *
*               -G G   :  gamma of generalized cepstrum (output)  [1.0] *
*               -N     :  regard output as normalized cepstrum          *
*               -U     :  regard output as multiplied by gamma          *
*       infile:                                                         *
*               generalized cepstrum                                    *
*                   , c(0), c(1), ..., c(m),                            *
*       stdout:                                                         *
*               generalized cepstrum                                    *
*                   , c'(0)(=c(0)), c'(1), ..., c'(M),                  *
*       notice:                                                         *
*               if g >= 1, g = -1 / g                                   *
*               if G >= 1, G = -1 / G                                   *
*       require:                                                        *
*               gc2gc(), gnorm(), ignorm()                              *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: gc2gc.c,v 1.12 2007/09/12 08:37:13 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define ORDER1 25
#define ORDER2 25
#define GAMMA1 0.0
#define GAMMA2 1.0
#define NORMFLG1 FA
#define NORMFLG2 FA
#define MULGFLG1 FA
#define MULGFLG2 FA

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
   fprintf(stderr, " %s - generalized cepstral transformation\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -m m  : order of generalized cepstrum (input)  [%d]\n", ORDER1);
   fprintf(stderr, "       -g g  : gamma of generalized cepstrum (input)  [%g]\n", GAMMA1);
   fprintf(stderr, "       -n    : regard input as normalized cepstrum    [%s]\n", BOOL[NORMFLG1]);
   fprintf(stderr, "       -u    : regard input as multiplied by gamma    [%s]\n", BOOL[MULGFLG1]);
   fprintf(stderr, "       -M M  : order of generalized cepstrum (output) [%d]\n", ORDER2);
   fprintf(stderr, "       -G G  : gamma of generalized cepstrum (output) [%g]\n", GAMMA2);
   fprintf(stderr, "       -N    : regard output as normalized cepstrum   [%s]\n", BOOL[NORMFLG2]);
   fprintf(stderr, "       -U    : regard output as multiplied by gamma   [%s]\n", BOOL[MULGFLG2]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       generalized cepstrum (%s)                   [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       transformed generalized cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       if g >= 1, g = -1 / g\n");
   fprintf(stderr, "       if G >= 1, G = -1 / G\n");
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
   int m1=ORDER1, m2=ORDER2, i;
   double *gc1, *gc2, g1=GAMMA1, g2=GAMMA2;
   Boolean norm1=NORMFLG1, norm2=NORMFLG2, mulg1=MULGFLG1, mulg2=MULGFLG2;
   FILE *fp=stdin;
    
   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'm':
            m1 = atoi(*++argv);
            --argc;
            break;
         case 'M':
            m2 = atoi(*++argv);
            --argc;
            break;
         case 'g':
            g1 = atof(*++argv);
            --argc;
            if (g1>=1.0) g1 = -1.0 / g1;
            break;
         case 'G':
            g2 = atof(*++argv);
            --argc;
            if (g2>=1.0) g2 = -1.0 / g2;
            break;
         case 'n':
            norm1 = 1 - norm1;
            break;
         case 'N':
            norm2 = 1 - norm2;
            break;
         case 'u':
            mulg1 = 1 - mulg1;
            break;
         case 'U':
            mulg2 = 1 - mulg2;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else 
         fp = getfp(*argv, "r");

   gc1 = dgetmem(m1+m2+2);
   gc2 = gc1 + m1 + 1;
    
   while (freadf(gc1, sizeof(*gc1), m1+1, fp)==m1+1) {
      if (!norm1) 
         gnorm(gc1, gc1, m1, g1);
      else if (mulg1)
         gc1[0] = (gc1[0] - 1.0) / g1;

      if (mulg1)
         for (i=m1; i>=1; i--) gc1[i] /= g1;

      gc2gc(gc1, m1, g1, gc2, m2, g2);

      if (!norm2)
         ignorm(gc2, gc2, m2, g2);
      else if (mulg1)
         gc2[0] = gc2[0] * g2 + 1.0;
   
      if (mulg2)
         for (i=m2; i>=1; i--) gc2[i] *= g2;

      fwritef(gc2, sizeof(*gc2), m2+1, stdout);
   }
   
   return 0;
}
