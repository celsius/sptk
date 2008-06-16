/* ----------------------------------------------------------------- */
/*             The Speech Signal Processing Toolkit (SPTK)           */
/*             developed by SPTK Working Group                       */
/*             http://sp-tk.sourceforge.net/                         */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 1984-2007  Tokyo Institute of Technology           */
/*                           Interdisciplinary Graduate School of    */
/*                           Science and Engineering                 */
/*                                                                   */
/*                1996-2008  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the SPTK working group nor the names of its */
/*   contributors may be used to endorse or promote products derived */
/*   from this software without specific prior written permission.   */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/****************************************************************

    $Id: _lspcheck.c,v 1.8 2008/06/16 05:48:46 heigazen Exp $

    Check order of LSP

       int lspcheck(lsp, ord)

       double   *lsp  : LSP
       int      ord   : order of LSP

       return   value : 0  -> normal
                        -1 -> ill condition

*****************************************************************/

int lspcheck (double *lsp, const int ord)
{
   int i;

   if ((lsp[0]<=0.0) || (lsp[0]>=0.5))
      return(-1);

   for (i=1; i<ord; i++) {
      if (lsp[i]<=lsp[i-1])
         return(-1);
      if ((lsp[i]<=0.0) || (lsp[i]>=0.5))
         return(-1);
   }
   
   return(0);
}

/****************************************************************

    $Id: _lspcheck.c,v 1.8 2008/06/16 05:48:46 heigazen Exp $

    Rearrangement of LSP

       void lsparng(lsp, ord)

       double    *lsp : LSP
       int        ord : order of LSP

*****************************************************************/

void lsparrange (double *lsp, const int ord)
{
   int i, flag;
   double tmp;

   /* check out of range */
   for (i=0; i<ord; i++) {
      if (lsp[i]<0.0)
         lsp[i] = -lsp[i];
      if (lsp[i]>0.5)
         lsp[i] = 1.0 - lsp[i];
   }

   /* check unmonotonic */
   for (;;) {
      flag = 0;
      for (i=1; i<ord; i++)
         if (lsp[i]<lsp[i-1]) {
            tmp = lsp[i];
            lsp[i] = lsp[i-1];
            lsp[i-1] = tmp;
            flag = 1;
         }

      if (!flag) break;
   }
   
   return;
}

