# ----------------------------------------------------------------
#       Speech Signal Processing Toolkit (SPTK): version 3.0
# 		       SPTK Working Group
# 
# 		 Department of Computer Science
# 		 Nagoya Institute of Technology
# 			      and
#   Interdisciplinary Graduate School of Science and Engineering
# 		 Tokyo Institute of Technology
# 		    Copyright (c) 1984-2000
# 		      All Rights Reserved.
# 
# Permission is hereby granted, free of charge, to use and
# distribute this software and its documentation without
# restriction, including without limitation the rights to use,
# copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of this work, and to permit persons to whom this
# work is furnished to do so, subject to the following conditions:
# 
#   1. The code must retain the above copyright notice, this list
#      of conditions and the following disclaimer.
# 
#   2. Any modifications must be clearly marked as such.
#                                                                        
# NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF TECHNOLOGY,
# SPTK WORKING GROUP, AND THE CONTRIBUTORS TO THIS WORK DISCLAIM
# ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT
# SHALL NAGOYA INSTITUTE OF TECHNOLOGY, TOKYO INSITITUTE OF
# TECHNOLOGY, SPTK WORKING GROUP, NOR THE CONTRIBUTORS BE LIABLE
# FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
# DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
# WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
# ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
# PERFORMANCE OF THIS SOFTWARE.
# ----------------------------------------------------------------

#
# Makefile for Imai Kobayashi Lab. command
#

PREFIX		= /home/alex/osigoto/Sourceforge/SPTK
BIN		= $(PREFIX)/bin
LIB		= $(PREFIX)/lib

CC		= gcc -ansi
RM		= rm -rf
INSTALL		= install -cs

INCDIR		= -I../include -I../../include
LIBDIR		= -L../../lib
CFLAGS		= $(INCDIR) -O2
LDFLAGS		= $(LIBDIR)
LIBS		= -lSPTK -lm
XLIBS		= -lX11



all:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" LIB="$(LIB)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" XLIBS="$(XLIBS)" DA_OSTYPE="$(DA_OSTYPE)") ; \
	done

install:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" XLIBS="$(XLIBS)" INSTALL="$(INSTALL)" BIN="$(BIN)" LIB="$(LIB)" DA_OSTYPE="$(DA_OSTYPE)" install ) ; \
	done

clean:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) RM="$(RM)" DA_OSTYPE="$(DA_OSTYPE)" clean ) ; \
	done
	$(RM) \#* *~

veryclean:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) RM="$(RM)" DA_OSTYPE="$(DA_OSTYPE)" veryclean ) ; \
	done
	$(RM) \#* *~

