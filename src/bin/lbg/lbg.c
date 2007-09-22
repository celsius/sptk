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
*     LBG Algorithm for Vector Quantizer Design                         *
*                                                                       *
*                                             1996. 4  K.Koishida       *
*                                                                       *
*        usage:                                                         *
*                lbg [ options ] [ indexfile ] < stdin > stdout         *
*        options:                                                       *
*                -l l      :  length of vector           [26]           *
*                -n n      :  order of vector            [25]           *
*                -t t      :  number of training vector  [N/A]          *
*                -s s      :  initial codebook size      [1]            *
*                -e e      :  final codebook size        [256]          *
*                -f f      :  initial codebook filename  [NULL]         *
*                (level 2)                                              *
*                -d d   :  end condition                 [0.0001]       * 
*                -r r   :  splitting factor              [0.0001]       *
*       infile:                                                         *
*                training vector (stdin)                                *
*                        x(0), x(1), ... x(t*l-1)                       *
*       stdout:                                                         *
*               trained codebook                                        *
*                        cb'(0), cb'(1), ... cb(l*e-1)                  *
*       ifile: (if s>1)                                                 *
*              initial codebook (cbfile)                                *
*                        cb(0), cb(1), ... cb(l*s-1)                    *
*       indexfile:                                                      *
*              VQ index of training vector (int)                        *
*                        index(0), index(1), ... index(t-1)             *
*       notice:                                                         *
*              codebook size (s and e) must be power of 2               *
*              -t option can be omitted, when input from redirect       *  
*      require:                                                         *
*              lbg(), vq()                                              *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id: lbg.c,v 1.13 2007/09/22 13:08:30 heigazen Exp $";


/*  Standard C Libraries  */
#include <stdio.h>
#include <SPTK.h>
#include <stdlib.h>
#include <string.h>


/*  Default Values  */
#define LENG 26
#define TNUMBER -1
#define ICBSIZE 1
#define ECBSIZE 256
#define DELTA 0.0001
#define END 0.0001

#define MAXVALUE 1e23
#define abs(x)  ( (x<0) ? (-(x)) : (x) )

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
   fprintf(stderr, " %s - LBG algorithm for vector quantizer design \n",cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ ifile ]<stdin>stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : length of vector          [%d]\n", LENG);
   fprintf(stderr, "       -n n  : order of vector           [%d]\n", LENG-1);
   fprintf(stderr, "       -t t  : number of training vector [N/A]\n");
   fprintf(stderr, "       -s s  : initial codebook size     [%d]\n", ICBSIZE);
   fprintf(stderr, "       -e e  : final codebook size       [%d]\n", ECBSIZE);
   fprintf(stderr, "       -f f  : initial codebook filename [NULL]\n");
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "     (level 2)\n");
   fprintf(stderr, "       -d d  : end condition             [%g]\n", END);
   fprintf(stderr, "       -r r  : splitting factor          [%g]\n", DELTA);
   fprintf(stderr, "  stdin:\n");
   fprintf(stderr, "       data sequence (%s)\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       codebook (%s)\n", FORMAT);
   fprintf(stderr, "  ifile:\n");
   fprintf(stderr, "       index (int)\n");
   fprintf(stderr, "  notice:\n");
   fprintf(stderr, "       codebook size (s and e) must be power of 2\n");
   fprintf(stderr, "       -t option can be omitted, when input from redirect\n");
#ifdef SPTK_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",SPTK_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}


int main (int argc, char **argv)
{
   int l=LENG, icbsize=ICBSIZE, ecbsize=ECBSIZE, tnum=TNUMBER, ispipe, xsize, csize, i, j, *tindex;
   FILE *fp=stdin, *fpi=NULL, *fpcb=NULL;
   double delta=DELTA, minerr=END, *x, *cb, *icb;
   double *p;

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
         case 't':
            tnum = atoi(*++argv);
            --argc;
            break;
         case 's':
            icbsize = atoi(*++argv);
            --argc;
            break;
         case 'e':
            ecbsize = atoi(*++argv);
            --argc;
            break;
         case 'd':
            minerr = atof(*++argv);
            --argc;
            break;
         case 'r':
            delta = atof(*++argv);
            --argc;
            break;
         case 'f':
            fpcb = getfp(*++argv, "r");
            --argc;
            break;
         case 'h':
            usage(0);
         default:
            fprintf(stderr, "%s : Invalid option '%c' !\n", cmnd, *(*argv+1));
            usage(1);
         }
      }
      else
         fpi = getfp(*argv, "w");

   if (tnum==-1) {
      ispipe = fseek(fp,0L,2);
      
#ifdef DOUBLE
      tnum = ftell(fp)/l/sizeof(double);
#else
      tnum = ftell(fp)/l/sizeof(float);
#endif /* DOUBLE */

      rewind(fp);
      if (ispipe==-1) {
         fprintf(stderr,"%s : -t option must be specified, when input via pipe!\n",cmnd);
         usage (1);
      }
   }

   xsize = tnum * l;
   csize = ecbsize * l;

   x = dgetmem(xsize);
   cb = dgetmem(csize);

   if (freadf(x, sizeof(*x), xsize, fp)!=xsize) {
      fprintf(stderr,"%s : Size error of training data !\n",cmnd);
      return(1);
   }

   if (icbsize==1) {
      icb=dgetmem(l);
      fillz(icb, sizeof(*icb), l);
      for (i=0,p=x; i<tnum; i++)
         for (j=0; j<l; j++)
            icb[j] += *p++;

      for (j=0; j<l; j++)
         icb[j] /= (double) tnum;
   }
   else {
      icb=dgetmem(icbsize*l);
      if (freadf(icb, sizeof(*icb), icbsize*l, fpcb)!=icbsize*l) {
         fprintf(stderr,"%s : Size error of initial codebook !\n",cmnd);
         return(1);
      }
   }

   lbg(x, l, tnum, icb, icbsize, cb, ecbsize, delta, minerr);

   fwritef(cb, sizeof(*cb), csize, stdout);

   if (fpi!=NULL) {
      tindex = (int *)dgetmem(tnum);
      for (i=0,p=x; i<tnum; i++,p+=l)
         tindex[i] = vq(p, cb, l, ecbsize);

      fwrite(tindex, sizeof(*tindex), tnum, fpi);
   }

   return(0);
}
