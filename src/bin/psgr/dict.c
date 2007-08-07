/*
  ----------------------------------------------------------------
 Speech Signal Processing Toolkit (SPTK): version 3.0
    SPTK Working Group

     Department of Computer Science
     Nagoya Institute of Technology
    and
    Interdisciplinary Graduate School of Science and Engineering
     Tokyo Institute of Technology
        Copyright (c) 1984-2000
   All Rights Reserved.

  Permission is hereby granted, free of charge, to use and
  distribute this software and its documentation without
  restriction, including without limitation the rights to use,
  copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of this work, and to permit persons to whom this
  work is furnished to do so, subject to the following conditions:

    1. The code must retain the above copyright notice, this list
       of conditions and the following disclaimer.

    2. Any modifications must be clearly marked as such.

  NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
  SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
  ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
  SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
  TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
  DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
 ----------------------------------------------------------------
*/

/* $Id: dict.c,v 1.4 2007/08/07 05:44:04 heigazen Exp $ */

#include <stdio.h>

void dict (void)
{
   puts("/BD{bind def}bind def");
   puts("/MT{moveto}BD");
   puts("/LT{lineto}BD");
   puts("/RMT{rmoveto}BD");
   puts("/RLT{rlineto}BD");
   puts("/ST{stroke}BD");

   puts("/FnC{/Courier}BD");
   puts("/FnCO{/Courier-Oblique}BD");
   puts("/FnCB{/Courier-Bold}BD");
   puts("/FnCBO{/Courier-BoldOblique}BD");
   puts("/FnS{/Symbol}BD");
   puts("/SF{");     /*  select font  */
   puts("  exch findfont exch");
   puts("  dup type /arraytype eq{makefont}{scalefont}ifelse");
   puts("  setfont");
   puts("}BD");

   puts("/MS{MT show}BD");
   puts("/RMS{RMT show}BD");
   puts("/GS{gsave}BD");
   puts("/GR{grestore}BD");
   puts("/NP{newpath}BD");

   /*  set rect path  */
   puts("/RP{");
   puts("dup type dup /integertype eq exch /realtype eq or {");
   puts("    4 -2 roll MT");
   puts("    dup 0 exch RLT exch 0 RLT");
   puts("    neg 0 exch RLT closepath");
   puts("  }{");
   puts("    dup length 4 sub 0 exch 4 exch");
   puts("    {");
   puts("      1 index exch 4 getinterval");
   puts("      aload pop");
   puts("      RP");
   puts("    }for");
   puts("    pop");
   puts("  }ifelse");
   puts("}BD");
   puts("/RS{GS NP RP stroke GR}BD");
   puts("/RF{GS NP RP fill GR}BD");
   puts("/RC{NP RP clip NP}BD");

   puts("/TR{translate}BD");
   puts("/SL{setlinewidth}BD");
   puts("/SG{setgray}BD");

   /*  fill pattern  */
   /* puts("/PM{32 32 1 [32 0 0 32 0 0]}BD");
    puts("/P2{<0102040801020408>}def");
    puts("/P3{<8040201008040201>}def");
    puts("/P4{<8888888888888888>}def");
    puts("/P5{<8142241812484281>}def");
    puts("/P6{<FF000000FF000000>}def");
    puts("/P7{<FF888888FF888888>}def");
    puts("/IM{image}BD");
   */
}
