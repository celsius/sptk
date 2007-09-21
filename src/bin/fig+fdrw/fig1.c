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

/****************************************************************
* NAME :         fig1 - subroutines for fig                     *
****************************************************************/

#include <string.h>
#include <math.h>
#include "fig.h"
#include "plot.h"

/*  Required Functions  */

#define GRKSIZE 62
static char *grk[]={"Alpha", "Beta", "Gamma", "Delta",
                    "Epsilon", "Zeta", "Eta", "Theta",
                    "Iota", "Kappa", "Lambda", "Mu",
                    "Nu", "Xi", " ", "Pi",
                    "Rho", "Sigma", "Tau", "Upsilon",
                    "Phi", "Chi", "Psi", "Omega",
                    " ", " ", " ", " ", " ", " ", " ", " ",
                    "alpha", "beta", "gamma", "delta",
                    "epsilon", "zeta", "eta", "theta",
                    "iota", "kappa", "lambda", "mu",
                    "nu", "xi", " ", "pi",
                    "rho", "sigma", "tau", "upsilon",
                    "phi", "chi", "psi", "omega",
                    " ", " ", " ", " ", " ", "infty"
                   };
static unsigned char sub[SBUFLNG];
static int nsub;

char *gettxt(char *s)
{
   char *p;
   int i, c;
   char *script(char *s, int i);

   while (*s && *s != '"')
      ++s;
   if (*s++=='\0')
      return(s - 1);
   for (p = s, nsub = i = 0; *s && (*s != '"' || *(s + 1)=='"'); ++s) {
      if (*s=='\\') { /* escape sequence */
         if ((c = greek(s + 1)) >= 0) {
            s += strlen(grk[c]);
            c += 193;
         }
         else {
            switch (c = *++s) {
            case 'b':
               c = 0x08;
               break;
            case 'n':
               c = 0x0a;
               break;
            case 'r':
               c = 0x0d;
               break;
            case '^':
               c = '^';
               break;
            case '_':
               c = '_';
               break;
            case '\\':
               c = '\\';
               break;
            case 'd':
            case 'u':
               s = script(s, i);
               c = ' ';
               break;
            default:
               sscanf(s, "%3d", &c);
               s += 2;
               break;
            }
         }
         p[i++] = c;
      }
      else if ((c = *s)=='^' || c=='_') {
         if (*(s + 1)=='{') {
            for (*++s = c; *(s + 1) != '}'; p[i++] = ' ') {
               s = script(s, i);
               *s = c;
            }
            ++s;
         }
         else {
            s = script(s, i);
            p[i++] = ' ';
         }
      }
      else {
         if (*s=='"')
            ++s;
         p[i++] = *s;
      }
   }
   sub[nsub] = p[i] = '\0';
   while (p + i<s)
      p[++i] = ' ';
   return(p);
}

char *script(char *s, int i)
{
   int c;

   sub[nsub++] = *s;
   sub[nsub++] = i;
   if ((c = *++s)=='\\') {
      if ((c = greek(s + 1)) >= 0) {
         s += strlen(grk[c]);
         c += 193;
      }
      else {
         switch (c = *++s) {
         case '\\':
         case '{':
         case '}':
         case '_':
         case '^':
            break;
         default:
            sscanf(s, "%3d", &c);
            s += 2;
            break;
         }
      }
   }
   sub[nsub++] = c;
   sub[nsub++] = '\0';
   return(s);
}

char *getarg(char *s, char *arg)
{
   if (s==NULL)
      return(s);
   while (*s==' ' || *s=='\t' || *s=='\n')
      ++s;
   if (*s=='\0')
      return(NULL);
   else if (*s=='"') {
      gettxt(s);
      while ((*arg++ = *s++))
         ;
   }
   else {
      while (*s != ' ' && *s != '\t' && *s != '\n' && *s != '\0')
         *arg++ = *s++;
      *arg = '\0';
   }
   return(s);
}

char *gettyp(char *s, char *t)
{
   char *p;

   s = getarg(p = s, t);
   if (isalpha(*t))
      return(s);
   else {
      strcpy(t, "lin");
      return(p);
   }
}

char *getname(char *s, char *t)
{
   if ((s = getarg(s, t))==NULL)
      *t = '\0';
   else if (*t=='"')
      *t++ = '\0';
   else {
      if (!is_number(*t))
         *t = '\0';
      t = gettxt(s);
   }
   return(t);
}

int greek(char *p)
{
   int n;

   for (n = 0; n<GRKSIZE; ++n)
      if (strncmp(p, grk[n], strlen(grk[n]))==0)
         return(n);
   return(-1);
}

double sleng(char *p, double h, double w)
{
   return((strlen(p) - 1) * w + LADJ * w);
}

void _symbol(double x, double y, char *p, double h, double w, double t)
{
   int i;
   double dx, dy;

   symbol(x, y, p, h, w, t);
   for (i = 0; sub[i]; i += 4) {
      dy = (sub[i]=='d' || sub[i]=='_') ? -h * 0.25 : h * 0.7;
      dx = sub[i + 1] * w;
      symbol(x + rx(dx, dy, t), y + ry(dx, dy, t), sub + i + 2, h * SSIZE, w, t);
   }
   sub[0] = 0;
}

double ysadj (void)
{
   int i;

   for (i = 0; sub[i]; i += 4)
      if (sub[i]=='u' || sub[i]=='^')
         return(1.5);

   return(0);
}

double rx(double x, double y, double t)
{
   t *= (3.141592653589793 / 180);
   return(x * cos(t) - y * sin(t));
}

double ry(double x, double y, double t)
{
   t *= (3.141592653589793 / 180);
   return(x * sin(t) + y * cos(t));
}

#define PI 3.141592653589793
#include <math.h>

double argapf(double x, double a)
{
   double omg;

   omg = PI * x;
   return(x + 2.0 * atan2(a * sin(omg), 1.0 - a * cos(omg)) / PI);
}
