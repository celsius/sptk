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
*    Mel-Generalized Cepstral Analysis                                  *
*                                                                       *
*                                       1990.7  K.Tokuda                *
*                                       1996.3  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*                mgcep [ options ] [ infile ] > stdout                  *
*       options:                                                        *
*                -a a     :  alpha                               [0.35] *
*                -g g     :  gamma                               [0]    *
*                -m m     :  order of mel-generalized cepstrum   [25]   *
*                -l l     :  frame length                        [256]  *
*                -o o     :  output format  (see stdout)         [0]    *
*                              0 (c~0...c~m)                            * 
*                              1 (b0...bm)                              *
*                              2 (K~,c~'1...c~'m)                       *
*                              3 (K,b'1...b'm)                          *
*                              4 (K~,g*c~'1...g*c~'m)                   *
*                              5 (K,g*b'1...g*b'm)                      *
*                (level 2)                                              *
*                -i i     :  minimum iteration                   [2]    *
*                -j j     :  maximum iteration                   [30]   *
*                -d d     :  end condition                       [0.001]*
*                -p p     :  order of recursions                 [l-1]  *
*                -e e     :  small value added to periodgram     [0]    *
*       infile:                                                         *
*                data sequence                                          *
*                        , x(0), x(1), ..., x(L-1),                     *
*       stdout:                                                         *
*                mel-generalized cepstrum (float)                       *
*       notice:                                                         *
*                if g >= 1.0, g = -1 / g                                *
*       require:                                                        *
*                mgcep()                                                *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: mgcep.c,v 1.19 2007/09/30 16:20:31 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SPTK.h>


/*  Default Values  */
#define ALPHA  0.35
#define GAMMA  0.0
#define ORDER  25
#define FLENG  256
#define OTYPE  0
#define MINITR 2
#define MAXITR 30
#define END    0.001
#define EPS    0.0

/*  Command Name  */
char *cmnd;


void usage (const int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - mel-generalized cepstral analysis\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -a a  : alpha                             [%g]\n", ALPHA);
   fprintf(stderr, "       -g g  : gamma                             [%g]\n", GAMMA);
   fprintf(stderr, "       -m m  : order of mel-generalized cepstrum [%d]\n", ORDER);
   fprintf(stderr, "       -l l  : frame length                      [%d]\n", FLENG);
   fprintf(stderr, "       -o o  : output format                     [%d]\n", OTYPE);
   fprintf(stderr, "                 0 (c~0...c~m)\n");
   fprintf(stderr, "                 1 (b0...bm)\n");
   fprintf(stderr, "                 2 (K~,c~'1...c~'m)\n");
   fprintf(stderr, "                 3 (K,b'1...b'm)\n");
   fprintf(stderr, "                 4 (K~,g*c~'1...g*c~'m)\n");
   fprintf(stderr, "                 5 (K,g*b'1...g*b'm)\n");
   fprintf(stderr, "     (level 2)\n");
   fprintf(stderr, "       -i i  : minimum iteration                 [%d]\n", MINITR);
   fprintf(stderr, "       -j j  : maximum iteration                 [%d]\n", MAXITR);
   fprintf(stderr, "       -d d  : end condition                     [%g]\n", END);
   fprintf(stderr, "       -p p  : order of recursions               [l-1]\n");
   fprintf(stderr, "       -e e  : small value added to periodgram   [%g]\n", EPS);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       windowed sequence (%s)                 [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       mel-generalized cepstrum (%s)\n", FORMAT);
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       if g >= 1.0, g = -1 / g\n");
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
   int m=ORDER, flng=FLENG, itr1=MINITR, itr2=MAXITR, n=-1, flag=0, otype=OTYPE, i;
   FILE *fp=stdin;
   double *b, *x, a=ALPHA, g=GAMMA, end=END, e=EPS, atof();

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
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
         case 'g':
            g = atof(*++argv);
            --argc;
            if (g >= 1.0) g = -1.0 / g;
            break;
         case 'm':
            m = atoi(*++argv);
            --argc;
            break;
         case 'l':
            flng = atoi(*++argv);
            --argc;
            break;
         case 'o':
            otype = atoi(*++argv);
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
         case 'p':
            n = atoi(*++argv);
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
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         fp = getfp(*argv, "r");

   if (n==-1)
      n = flng-1;

   x = dgetmem(flng+m+m+2);
   b = x+flng;

   while (freadf(x, sizeof(*x), flng, fp)==flng) {
      flag = mgcep(x, flng, b, m, a, g, n, itr1, itr2, end, e);

      if (otype==0 || otype==1 || otype==2 || otype==4)
         ignorm(b, b, m, g);  /* K, b'r --> br  */

      if (otype==0 || otype==2 || otype==4)
         if (a!=0.0) b2mc(b, b, m, a);  /* br --> c~r */

      if (otype==2 || otype==4)
         gnorm(b, b, m, g);  /* c~r --> K~, c~'r */

      if (otype==4 || otype==5)
         for (i=m; i>=1; i--) b[i] *= g;

      fwritef(b, sizeof(*b), m+1, stdout);
   }
 
   return 0;
}
