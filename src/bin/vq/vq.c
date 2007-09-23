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
*     Vector Quantization                                               *
*                                                                       *
*                                           1995.12  K.Koishida         *
*                                                                       *
*        usage:                                                         *
*                vq [ options ] cbfile [ infile ] > stdout              *
*        options:                                                       *
*                -l l :  length of vector           [26]                *   
*                -n n :  order of vector            [25]                *
*                -q   :  output quantized vector    [FALSE]             *
*        infile:                                                        *
*                input vector                                           *
*                        x(0), x(1), ...                                *
*                codebook vector                                        *
*                        cb(0), cb(1), ... cb(l*c-1)                    *
*        stdout:                                                        *
*                index of codebook (int)                                *
*                        index(0), index(1), ...,                       *
*                quantized vector (if -q option is specified)           *
*                        x'(0), x'(1), ...                              *
*       require:                                                        *
*                vq()                                                   *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <stdlib.h>
#include <string.h>


/*  Default Values  */
#define LENG   26
#define CBSIZE 256
#define QFLAG  FA

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
   fprintf(stderr, " %s - vector quantization\n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] cbfile [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : length of vector        [%d]\n", LENG);
   fprintf(stderr, "       -n n  : order of vector         [%d]\n", LENG-1);
   fprintf(stderr, "       -q    : output quantized vector [%s]\n", BOOL[QFLAG]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       vectors (%s)                 [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       index (int) or\n");
   fprintf(stderr, "       quantized vectors (%s) if -q option is specified\n", FORMAT);
   fprintf(stderr, "  cbfile:\n");
   fprintf(stderr, "       codebook (%s)\n", FORMAT);
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
   int l=LENG, cbsize=CBSIZE, index;
   Boolean qflag=QFLAG;
   FILE *fp=stdin, *fpcb=NULL;
   double *x, *qx, *cb;

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc)
      if (**++argv=='-') {
         switch (*(*argv+1)) {
         case 'l':
            l = atoi(*++argv);
            --argc;
            break;
         case 'n':
            l = atoi(*++argv)+1;
            --argc;
            break;
         case 's':
            cbsize = atoi(*++argv);
            --argc;
            break;
         case 'q':
            qflag = 1 - qflag;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage (1);
         }
      }
      else if (fpcb==NULL)
         fpcb = getfp(*argv, "r");
      else
         fp = getfp(*argv, "r");

   fseek(fpcb,0,2);
#ifdef DOUBLE
   cbsize = ftell(fpcb)/sizeof(double)/l;
#else
   cbsize = ftell(fpcb)/sizeof(float)/l;
#endif  /* DOUBLE */
   rewind(fpcb);

   x = dgetmem(l+l+cbsize*l);
   qx = x + l;
   cb = qx + l;

   if (freadf(cb, sizeof(*cb), cbsize*l, fpcb)!=cbsize*l) {
      fprintf(stderr,"%s : Codebook size error !\n",cmnd);
      return(1);
   }

   if (! qflag)
      while (freadf(x, sizeof(*x), l, fp)==l) {
         index = vq(x, cb, l, cbsize);
         fwrite(&index, sizeof(index), 1, stdout);
      }
   else
      while (freadf(x, sizeof(*x), l, fp)==l) {
         index = vq(x, cb, l, cbsize);
         ivq(index, cb, l, qx);
         fwritef(qx, sizeof(*qx), l, stdout);
      }

   return(0);
}

