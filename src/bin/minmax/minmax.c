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
*    Find Minimum and Maximum Values                                    *
*                                                                       *
*                                       1988.7 T.Kobayashi              *
*                                       1996.1 N.Isshiki                *
*                                       1998.7 M.Tamura                 *
*                                                                       *
*       usage:                                                          *
*               minmax [ options ] [ infile ] > stdout                  *
*       options:                                                        *
*               -l l  :  length of vector     [1]                       *
*               -n n  :  order of vector      [l-1]                     *
*               -b b  :  find n-best values   [1]                       *
*               -d d  :  output data number   [FALSE]                   *
*       infile:                                                         *
*               data sequence (float)                                   *
*       stdout:                                                         *
*               minimum & maximum(float)                                *
*               n-best minimums & n-best maximum (float)                *
*                       ,when -n option is specified                    *
*               minimum:datanumber,... (ascii)                          *
*               maximum:datanumber,...  (ascii)                         *
*                       ,when -d option is specified                    *
*                                                                       *
************************************************************************/

static char *rcs_id = "$Id$";


/* Standard C Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#include "SPTK.h"
#else
#include <SPTK.h>
#endif

/* Defualt Values */
#define DIM 1
#define NBEST 1
#define FLT_MAX 3.4e+38
#define FLT_MIN -3.4e+38
#define OUTNUM FA

char *BOOL[] = {"FALSE", "TRUE"};

/* Command Name */
char *cmnd;


void usage (int status)
{
   fprintf(stderr, "\n");
   fprintf(stderr, " %s - find minimum and maximum values\n", cmnd);
   fprintf(stderr, "\n");
   fprintf(stderr, "  usage:\n");
   fprintf(stderr, "       %s [ options ] [ infile ] > stdout\n", cmnd);
   fprintf(stderr, "  options:\n");
   fprintf(stderr, "       -l l  : length of vector   [%d]\n",DIM);
   fprintf(stderr, "       -n n  : order of vector    [l-1]\n");
   fprintf(stderr, "       -b b  : find n-best values [%d]\n",NBEST);
   fprintf(stderr, "       -d    : output data number [%s]\n",BOOL[OUTNUM]);
   fprintf(stderr, "       -h    : print this message\n");
   fprintf(stderr, "  infile:\n");
   fprintf(stderr, "       data sequence (%s)      [stdin]\n", FORMAT);
   fprintf(stderr, "  stdout:\n");
   fprintf(stderr, "       minimum and maximum values (%s)\n", FORMAT);
   fprintf(stderr, "       or \"values:datanumber,...\" (ascii)\n");
   fprintf(stderr, "       ,if -d option is specified\n");
#ifdef PACKAGE_VERSION
   fprintf(stderr, "\n");
   fprintf(stderr, " SPTK: version %s\n",PACKAGE_VERSION);
   fprintf(stderr, " CVS Info: %s", rcs_id);
#endif
   fprintf(stderr, "\n");
   exit(status);
}

int main (int argc,char *argv[])
{
   FILE *fp;
   char *s, *infile=NULL, c;
   Boolean outnum=OUTNUM;
   int dim=DIM,nbest=NBEST;
   int minmax (FILE *fp,int dim,int nbest,Boolean outnum);

   if ((cmnd=strrchr(argv[0], '/'))==NULL)
      cmnd = argv[0];
   else
      cmnd++;
   while (--argc) {
      if (*(s=*++argv)=='-') {
         c = *++s;
         if (*++s=='\0' && ((c=='l') || (c=='n') || ( c=='b'))) {
            s = *++argv;
            --argc;
         }
         switch (c) {
         case 'l':
            dim = atoi(s);
            break;
         case 'n':
            dim = atoi(s)+1;
            break;
         case 'b':
            nbest = atoi(s);
            break;
         case 'd':
            outnum = 1 - outnum;
            break;
         case 'h':
            usage (0);
         default:
            fprintf(stderr, "%s : Invalid option '%c'!\n", cmnd, c);
            usage (1);
            break;
         }
      }
      else
         infile = s;
   }
   if (infile) {
      fp = getfp(infile, "r");
   }
   else
      fp = stdin;
   minmax(fp,dim,nbest, outnum);
   
   return(0);
}

int minmax (FILE *fp,int dim,int nbest,Boolean outnum)
{
   double *s;
   int k, n, i, j;
   int **minpos=NULL,**maxpos=NULL,*nmaxpos=NULL,*nminpos=NULL,*tmp, t=0;
   double *min,*max;

   min = dgetmem(nbest);
   max = dgetmem(nbest);
   s = dgetmem(dim);
   if(outnum){
      minpos = (int **)calloc(sizeof(*minpos),nbest);
      maxpos = (int **)calloc(sizeof(*maxpos),nbest);
      nminpos = ( int *)calloc(sizeof(*nminpos),nbest);
      nmaxpos = ( int *)calloc(sizeof(*nminpos),nbest);
   }
   for (k=0; !feof(fp); ) {
      if ((n=freadf(s, sizeof(*s), dim, fp))==0)
         break;
      if (dim!=1 || k==0) {
         for (i=1;i<nbest;i++) {
            min[i] = FLT_MAX;
            max[i] = FLT_MIN;
         }
         min[0]=s[0];
         max[0]=s[0];
         if(outnum){
            for (i=0;i<nbest;i++) {
               nminpos[i] = 1;
               nmaxpos[i] = 1;
               free(minpos[i]);
               free(maxpos[i]);
               minpos[i] = (int *)calloc(sizeof(**minpos),1);
               maxpos[i] = (int *)calloc(sizeof(**maxpos),1);
               minpos[i][0] = -1;
               maxpos[i][0] = -1;
            }	 
            minpos[0][0] = 0;
            maxpos[0][0] = 0;
         }
      }    
      else {
         for (i=0;i<nbest;i++) { 
            if (s[0]==min[i] && outnum) {
               tmp = (int *)calloc(sizeof(*tmp),nminpos[i]);
               movem((double *)minpos[i],(double *)tmp,sizeof(*tmp),nminpos[i]);
               free(minpos[i]);
               minpos[i]=(int *)calloc(sizeof(**minpos),++nminpos[i]);
               movem((double *)tmp,(double *)minpos[i],sizeof(**minpos),nminpos[i]-1);
               minpos[i][nminpos[i]-1] = t;
               free(tmp);
               break;	      
            }
            if (s[0]<min[i]) {
               if(outnum)
                  free(minpos[nbest-1]);
               for (j=nbest-1;j>i;j--) {
                  min[j]=min[j-1];
                  if(outnum){
                     minpos[j] = minpos[j-1];
                     nminpos[j] = nminpos[j-1];
                  }
               }
               min[i] = s[0];
               if(outnum){
                  minpos[i] = (int *)calloc(sizeof(**minpos),1);
                  minpos[i][0] = t;
                  nminpos[i] = 1;
               }
               break;
            }
         }
         for (i=0;i<nbest;i++) {
            if (s[0]==max[i] && outnum) {
               tmp = (int *)calloc(sizeof(*tmp),nmaxpos[i]);
               movem((double *)maxpos[i],(double *)tmp,sizeof(*tmp),nmaxpos[i]);
               free(maxpos[i]);
               maxpos[i]=(int *)calloc(sizeof(**maxpos),++nmaxpos[i]);
               movem((double *)tmp,(double *)maxpos[i],sizeof(**maxpos),nmaxpos[i]-1);
               maxpos[i][nmaxpos[i]-1]=t;
               free(tmp);
               break;
            }
            if (s[0]>max[i]) {
               if(outnum)
                  free(maxpos[nbest-1]);
               for (j=nbest-1;j>i;j--) {
                  max[j]=max[j-1];
                  if(outnum){
                     maxpos[j] = maxpos[j-1];
                     nmaxpos[j] = nmaxpos[j-1];
                  }
               }
               max[i] = s[0];
               if(outnum){
                  maxpos[i] = (int *)calloc(sizeof(**minpos),1);
                  maxpos[i][0] = t;
                  nmaxpos[i] = 1;
               }
               break;
            }
         }
      }
      for (k=1; k<n; ++k) {
         for (i=0;i<nbest;i++) {
            if (s[k]==min[i] && outnum) {
               tmp = (int *)calloc(sizeof(*tmp),nminpos[i]);
               movem((double *)minpos[i],(double *)tmp,sizeof(*tmp),nminpos[i]);
               free(minpos[i]);
               minpos[i]=(int *)calloc(sizeof(**minpos),++nminpos[i]);
               movem((double *)tmp,(double *)minpos[i],sizeof(**minpos),nminpos[i]-1);
               minpos[i][nminpos[i]-1]=k;
               free(tmp);
               break;
            }
            if (s[k]<min[i]) {
               if(outnum)
                  free(minpos[nbest-1]);
               for (j=nbest-1;j>i;j--) {
                  min[j]=min[j-1];
                  if(outnum){
                     minpos[j] = minpos[j-1];
                     nminpos[j] = nminpos[j-1];
                  }
               }
               min[i] = s[k];
               if(outnum){
                  minpos[i] = (int *)calloc(sizeof(**minpos),1);
                  minpos[i][0] = k;
                  nminpos[i] = 1;
               }
               break;
            }
         }
         for (i=0;i<nbest;i++) {
            if (s[k]==max[i] && outnum) {
               tmp = (int *)calloc(sizeof(*tmp),nmaxpos[i]);
               movem((double *)maxpos[i],(double *)tmp,sizeof(*tmp),nmaxpos[i]);
               free(maxpos[i]);
               maxpos[i]=(int *)calloc(sizeof(**maxpos),++nmaxpos[i]);
               movem((double *)tmp,(double *)maxpos[i],sizeof(**maxpos),nmaxpos[i]-1);
               maxpos[i][nmaxpos[i]-1]=k;
               free(tmp);
               break;
            }
            if (s[k]>max[i]) {
               if(outnum)
                  free(maxpos[nbest-1]);
               for (j=nbest-1;j>i;j--) {
                  max[j]=max[j-1];
                  if(outnum){
                     maxpos[j] = maxpos[j-1];
                     nmaxpos[j] = nmaxpos[j-1];
                  }
               }
               max[i] = s[k];
               if(outnum){
                  maxpos[i] = (int *)calloc(sizeof(**maxpos),1);
                  maxpos[i][0] = k;
                  nmaxpos[i] = 1;
               }
               break;
            }
         }
      }
      if (dim!=1) {
         if (outnum) {
            for (i=0;i<nbest;i++) {
               printf("%g:",min[i]);
               printf("%d",minpos[i][0]);
               for (j=1;j<nminpos[i];j++)
                  printf(",%d",minpos[i][j]);
               printf("\n");
            }
            for (i=0;i<nbest;i++) {
               printf("%g:",max[i]);
               printf("%d",maxpos[i][0]);
               for (j=1;j<nmaxpos[i];j++)
                  printf(",%d",maxpos[i][j]);
               printf("\n");
            }
         }
         else {
            fwritef(min, sizeof(*min), nbest, stdout);
            fwritef(max, sizeof(*max), nbest, stdout);
         }
      }
      else
         t++;
   }
   if (dim==1) {
      if (outnum) {
         for (i=0;i<nbest;i++) {
            printf("%g:",min[i]);
            printf("%d",minpos[i][0]);
            for (j=1;j<nminpos[i];j++)
               printf(",%d",minpos[i][j]);
            printf("\n");
         }
         for (i=0;i<nbest;i++) {
            printf("%g:",max[i]);
            printf("%d",maxpos[i][0]);
            for (j=1;j<nmaxpos[i];j++)
               printf(",%d",maxpos[i][j]);
            printf("\n");
         }
      }
      else {
         fwritef(min, sizeof(*min), nbest, stdout);
         fwritef(max, sizeof(*max), nbest, stdout);
      }
   }
   return(0);
}
