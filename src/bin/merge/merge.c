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
*    Data Merge                                                         *
*                                                                       *
*                                       1986.6  K.Tokuda                *
*                                       1996.5  K.Koishida              *
*                                                                       *
*       usage:                                                          *
*               merge [options] file1 [infile] > stdout                 *
*       options:                                                        *
*               -s s     :  insert point                        [0]     *
*               -l l     :  frame length of input data          [25]    *
*               -n n     :  order of input data                 [l-1]   *
*               -L L     :  frame length of insert data         [10]    *
*               -N N     :  order of insert data                [L-1]   *
*               -o       :  over write mode                     [FALSE] *
*               +type    :  data type                           [f]     *
*                          c (char)     s (short)                       *
*                          i (int)      l (long)                        *
*                          f (float)    d (double)                      *
*                                                                       *
*       file1:  inserted data   , x(0), x(1), ..., x(l-1)               *
*       file2:  input data      , y(0), y(1), ..., y(n-1)               *
*       stdout:                                                         *
*               x(0), ..., x(s), y(0), ...,y(n-1), x(s+1), ..., x(n-1)  *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <SPTK.h>


/*  Default Values  */
#define START 0
#define LENG1 25
#define LENG2 10
#define WRITE FA

char *BOOL[] = {"FALSE", "TRUE"};

/*  Command Name  */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - data merge\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] file1 [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -s s  : insert point                [%d]\n",START);
   fprintf(stderr, "       -l l  : frame length of input data  [%d]\n",LENG1);
   fprintf(stderr, "       -n n  : order of input data         [l-1]\n");
   fprintf(stderr, "       -L L  : frame length of insert data [%d]\n",LENG2);
   fprintf(stderr, "       -N N  : order of insert data        [L-1]\n");
   fprintf(stderr, "       -o    : over write mode             [%s]\n",BOOL[WRITE]);
   fprintf(stderr, "       +type : data type                   [f]\n");
   fprintf(stderr, "                c (char)      s (short)\n");
   fprintf(stderr, "                i (int)       l (long)\n");
   fprintf(stderr, "                f (float)     d (double)\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence                       [stdin]\n");
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       merged data sequence\n");
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
   FILE *fp2=NULL, *fp1=stdin;
   int start=START, leng1=LENG1, leng2=LENG2, i, j, flag=1;
   size_t size=sizeof(float);
   Boolean write=WRITE;
   char *y, c, *s;
   double x;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (*(s=*++argv)=='-') {
         c = *++s;
         switch (c) {
         case 's':
            start = atoi(*++argv);
            --argc;
            break;
         case 'l':
            leng1 = atoi(*++argv);
            --argc;
            break;
         case 'n':
            leng1 = atoi(*++argv)+1;
            --argc;
            break;
         case 'L':
            leng2 = atoi(*++argv);
            --argc;
            break;
         case 'N':
            leng2 = atoi(*++argv)+1;
            --argc;
            break;
         case 'o':
            write = 1 - write;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else if (*s=='+') {
         c = *++s;
         switch (c) {
         case 'c':
            size = sizeof(char);
            break;
         case 's':
            size = sizeof(short);
            break;
         case 'l':
            size = sizeof(long);
            break;
         case 'i':
            size = sizeof(int);
            break;
         case 'f':
            size = sizeof(float);
            break;
         case 'd':
            size = sizeof(double);
            break;
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else if (fp2==NULL)
         fp2 = getfp(*argv, "r");
      else
         fp1 = getfp(*argv, "r");

   y = (char *)dgetmem(leng2 * size);

   for (; ;) {
      for (j=start,i=leng1; j-- && i--;) {
         if (fread(&x, size, 1, fp1)!=1)
            break;
         fwrite(&x, size, 1, stdout);
      }
      for (j=leng2; j--;)
         if (write) {
            if (fread(&x, size, 1, fp1)!=1)
               break;
            i--;
         }
      if (fread(y, size, leng2, fp2)!=leng2)
         if (!flag)
            break;

      fwrite(y, size, leng2, stdout);
      flag = 0;
      for (; i-->0;) {
         if (fread(&x, size, 1, fp1)!=1)
            break;
         fwrite(&x, size, 1, stdout);
      }
   }
   
   if (feof(fp1) && feof(fp2))
      return(0);
   else
      return(1);
}
