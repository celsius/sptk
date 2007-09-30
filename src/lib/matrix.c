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

/*****************************************************************

    $Id$

    compute matrix functions

    multiple matrix
    multim(x, xx, xy, y, yx, yy, a)

        double x[] : matrix1
        int     xx : row number of matrix1
        int     xy : line number of matrix1
        double y[] : matrix2
        int     yx : row number of matrix2
        int     yy : line number of matrix2
        double a[] : answer

        if xx=1 and xy=1 ,then x[0] is regarded as scalar
		
    add matrix
    addm(x, y, xx, yy, a)
        double x[] : matrix1
        double y[] : matrix2
        int     xx : row number 
        int     xy : line number
        double a[] : answer

                        Naohiro Isshiki Feb.1996
*******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <SPTK.h>

static double *tmp;
static int tmpsize = 0;

static void mm (double x[], const int xx, const int xy, double y[], const int yx, const int yy, double a[])
{	
   int i,j,k;
   double *wx,*wy;
   
   if (xx==1 && xy==1) {
      for (i=yx*yy-1; i>=0; i--)
         a[i] = x[0]*y[i];
      return;
   }

   if (xx != yy) {
      fprintf(stderr,"Invalid matrix size x= %d*%d,y= %d*%d\n",xx,xy,yx,yy);
      exit(1);
   }

   wx = x;
   for (i=0; i<xy; i++) {
      for (j=0; j<yx; j++) {
         wy = &y[j];
         *a=0;
         for (k=0; k<xx; k++) {
            *a += *wx * *wy ;
            wx++;
            wy += yx;
         }
         wx -= xx; 
         a++;
      }
      wx += xx;
   }

   return;
}

void multim (double x[], const int xx, const int xy, double y[], const int yx, const int yy, double a[])
{
   int i;

   if (x==a) {
      if (((xy>yy) ? xy : yy)*yx>tmpsize) {
         if (tmp != NULL)
            free(tmp);
         tmpsize = ((xy>yy) ? xy : yy)*yx;
         tmp = (double *)getmem(tmpsize, sizeof(*tmp));
      }
      mm(x,xx,xy,y,yx,yy,tmp);
      if (xx==xy)
         for (i=yx*yy-1; i>=0; i--)
            a[i] = tmp[i]; 
      else
         for (i=xy*yx-1; i>=0 ; i--)
            a[i] = tmp[i]; 
   }
   else {
      mm(x,xx,xy,y,yx,yy,a);
   }

   return;
}

static void am (double x[], double y[], const int xx, const int yy, double a[])
{
   int i,j;

   for (i=0; i<yy; i++)
      for (j=0; j<xx; j++)
         a[j+i*xx] = x[j+i*xx] + y[j+i*xx];
}

void addm (double x[], double y[], const int xx, const int yy, double a[])
{
   int i;

   if (x==a) {
      if (xx*yy>tmpsize) { 
         if (tmp!=NULL)
            free(tmp);
         tmpsize = xx * yy;
         tmp = (double *)getmem(tmpsize, sizeof(*tmp));
      }
      am(x,y,xx,yy,tmp);
      for (i=xx*yy-1; i>=0; i--)
         a[i] = tmp[i]; 
   }
   else {
      am(x,y,xx,yy,a);
   }

   return;
}
