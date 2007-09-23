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
*    Execute Vector Operations                                          *
*                                                                       *
*                                               1988.6  T.Kobayashi     *
*                                               1996.5  K.Koishida      *
*       usage:                                                          *
*               vopr [ options ] [ [ file1 ] [ infile ]  > stdout       *
*       options:                                                        *
*               -l l     :  length of vector                [1]         *
*               -n n     :  order of vector                 [l-1]       *
*               -i       :  specified file contains a and b [FALSE]     *
*               -a       :  addition       (a + b)          [FALSE]     * 
*               -s       :  subtraction    (a - b)          [FALSE]     *
*               -m       :  multiplication (a * b)          [FALSE]     *
*               -d       :  division       (a / b)          [FALSE]     *
*               -ATAN2   :  atan2          atan2(b,a)       [FALSE]     *
*        notice:                                                        *
*               When both -l and -n are specified,                      *
*               latter argument is adopted.                             *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <SPTK.h>


/*  Default Values  */
#define LENG 1
#define INV  FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - excute vector operations\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ file1 ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l   : length of vector                [%d]\n",LENG);
   fprintf(stderr, "       -n n   : order of vector                 [l-1]\n");
   fprintf(stderr, "       -i     : specified file contains a and b [%s]\n",BOOL[INV]);
   fprintf(stderr, "       -a     : addition       (a + b)          [FALSE]\n");
   fprintf(stderr, "       -s     : subtraction    (a - b)          [FALSE]\n");
   fprintf(stderr, "       -m     : multiplication (a * b)          [FALSE]\n");
   fprintf(stderr, "       -d     : division       (a / b)          [FALSE]\n");
   fprintf(stderr, "       -ATAN2 : atan2          atan2(b,a)       [FALSE]\n");
   fprintf(stderr, "       -h     : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data vectors (%s)                     [stdin]\n", FORMAT);
   fprintf(stderr, "  file1:\n");
   fprintf(stderr, "       data vectors (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       data vectors after operations (float)\n");
   fprintf(stderr, "  note:\n");
   fprintf(stderr, "       When both -l and -n are specified,\n");
   fprintf(stderr, "       latter argument is adopted.\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int opr=' ', leng=LENG;
Boolean inv=INV;

int main (int argc, char **argv)
{
   int nfiles=0;
   FILE *fp1, *fp2;
   char *s, c, *infile[4];
   int vopr(FILE *fp1,FILE *fp2);
   
   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc) {
      if (*(s=*++argv)=='-') {
         c = *++s;
         if (*++s=='\0' && (c=='l' || c=='n')) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            leng = atoi(s);
            break;
         case 'n':
            leng = atoi(s)+1;
            break;
         case 'i':
            inv = 1 - inv;
            break;
         case 'a':
         case 'd':
         case 'm':
         case 's':
         case 'A':
            opr = c;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else
         infile[nfiles++] = s;
   }

   if (nfiles==0)
      vopr(stdin, stdin);
   else {
      fp1 = getfp(infile[0], "r");
      if (nfiles==1) {
         if (inv)
            vopr(fp1, fp1);
         else
            vopr(stdin, fp1);
      }
      else {
         fp2 = getfp(infile[1], "r");
         vopr(fp1, fp2);
      }
   }

   return(0);
}

int vopr (FILE *fp1, FILE *fp2)
{
   double *a, *b;
   int k;

   a = dgetmem(leng + leng);
   b = a + leng;

   if (fp1!=fp2 && leng>1) {
      if (freadf(b, sizeof(*b), leng, fp2)!=leng)
         return(1);
   }
   while (freadf(a, sizeof(*a), leng, fp1)==leng) {
      if (fp1==fp2 || leng==1) {
         if (freadf(b, sizeof(*b), leng, fp2)!=leng)
            return(1);
      }

      switch (opr) {
      case 'a':
         for (k=0; k<leng; ++k)
            a[k] += b[k];
         break;
      case 's':
         for (k=0; k<leng; ++k)
            a[k] -= b[k];
         break;
      case 'm':
         for (k=0; k<leng; ++k)
            a[k] *= b[k];
         break;
      case 'd':
         for (k=0; k<leng; ++k)
            a[k] /= b[k];
         break;
      case 'A':
         for (k=0; k<leng; ++k)
            a[k] = atan2(b[k], a[k]);
         break;
      default:
         break;
      }

      fwritef(a, sizeof(*a), leng, stdout);
   }
   return(0);
}

