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

/**************************************************************************
*                                                                         *
*    Digital Filter in Standard Form                                      *
*                                                                         *
*                                       1989.6   K.Tokuda                 *
*                                       1995.12  N.Isshiiki modified      *
*       usage:                                                            *
*               dfs [ options ] [ infile ] > stdout                       *
*       options:                                                          *
*               -a k a1 a2 ... aM  : denominator coefficients      [N/A]  *
*               -b b0 b1 b2 ... bM : numerator coefficients        [N/A]  *
*               -p pfile           : denominator coefficients file [NULL] *
*               -z zfile           : numerator coefficients file   [NULL] *
*       infile:                                                           *
*               input (float)                                             *
*       stdout:                                                           *
*               output (float)                                            *
*       note:                                                             *
*               M, N <= 2047                                              *
**************************************************************************/

static char *rcs_id = "$Id: dfs.c,v 1.14 2007/09/23 15:08:27 heigazen Exp $";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <SPTK.h>

/* Default Values */
#define SIZE 2048

/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - digital filter in standard form\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout \n", cmnd);
   fprintf(stderr, "  options:\n");           
   fprintf(stderr, "       -a K  a1...aM : denominator coefficients      [N/A]\n");
   fprintf(stderr, "       -b b0 b1...bN : numerator coefficients        [N/A]\n");
   fprintf(stderr, "       -p pfile      : denominator coefficients file [NULL]\n");
   fprintf(stderr, "       -z zfile      : numerator coefficients file   [NULL]\n");
   fprintf(stderr, "       -h            : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       filter input (%s)                          [stdin]\n", FORMAT); 
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       filter output (%s)\n", FORMAT);
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       M,N <= %d \n",SIZE-1);
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n", PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc, char *argv[])
{
   int i;
   static double a[SIZE], b[SIZE];
   static double d[SIZE];
   int bufp = 0;
   int na=-1, nb=-1;
   double x;
   char *file_z="", *file_p="";
   FILE *fp_z, *fp_p;

   if ((cmnd = strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-')
         switch (*(*argv+1)) {
         case 'a':
            i = 0;
            while ( (argc-1) && !isalpha(*(*(argv+1)+1)) ) {
               a[i++] = atof(*++argv);
               argc--;
               na++;
            }
            break;
         case 'b':
            i = 0;
            while ( (argc-1) && !isalpha(*(*(argv+1)+1)) ) {
               b[i++] = atof(*++argv);
               argc--;
               nb++;
            }
            break;
         case 'z':
            argc--;
            file_z = *++argv;
            break;
         case 'p':
            argc--;
            file_p = *++argv; 
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : invalid option !\n", cmnd);
            usage(1);
         }
      else {
         fprintf(stderr, "%s : invalid option !\n", cmnd);
         usage(1);
      }

   if (*file_z!='\0') {
      fp_z = getfp(file_z, "r");
      nb = freadf(b, sizeof(*b), SIZE, fp_z) - 1;
   }
   if (*file_p!='\0') {
      fp_p = getfp(file_p, "r");
      na = freadf(a, sizeof(*a), SIZE, fp_p) - 1;
   }

   if (na==-1) {
      na = 0;
      a[0] = 1.0;
   }
   if (nb==-1) {
      nb = 0;
      b[0] = 1.0;
   }

   while (freadf(&x, sizeof(x), 1, stdin)==1) {
      x = dfs(x, a, na, b, nb, d, &bufp);
      fwritef(&x, sizeof(x), 1, stdout);
   }
   
   return 0;
}
