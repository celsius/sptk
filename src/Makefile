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

########################################################################
#
# Makefile for Speech Signal Processing Toolkit
#
########################################################################
#
# Location to install programs, libraries, and include files.
#
PREFIX = /usr/local/SPTK
BIN = $(PREFIX)/bin
LIB = $(PREFIX)/lib
INCLUDE = $(PREFIX)/include

#
# Directories which contain libraries and include files of X11.
#
XINCDIR = /usr/X11R6/include
XLIBDIR = /usr/X11R6/lib
XLIBS	= -lX11
#XLIBS = -lX11 -lsocket

#
# Compiler and Instllation programs.
# Some systems should replace 'install -cs' with 'cp'.
#
CC	= gcc
RM	= rm -rf
INSTALL = install -cs

#
# Uncomment if you want to deal with data in double
#
#DOUBLE		= -DDOUBLE

#
# If you want to compile 'da' and 'daplay', specify your operating system.
# These commands will work only on SunOS4.1.x, SunOS5.x(sparc), and linux(i386).
# Avaliable values are:
#    SUNOS          SunOS4.1.x
#    SOLARIS        SunOS5.x(sparc)
#    LINUX          linux(i386)
#    FreeBSD        FreeBSD(i386 newpcm driver)
# 
DA_OSTYPE = 

########################################################################

SUBDIR = lib include bin script

INCDIR = -I../include -I../../include -I$(XINCDIR)
LIBDIR = -L../../lib -L$(XLIBDIR)
CFLAGS = -O2 -ansi -Wall -DSPTK_VERSION=\"3.1\" $(INCDIR) $(DOUBLE)
LDFLAGS = $(LIBDIR)
LIBS = -lSPTK -lm


all:
	for d in $(SUBDIR) ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" LIB="$(LIB)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" XLIBS="$(XLIBS)" DA_OSTYPE="$(DA_OSTYPE)") ; \
	done

install:
	for d in $(PREFIX) $(BIN) $(LIB) $(INCLUDE) ; do \
		if [ ! -d $$d ]; then \
			rm -rf $$d ; \
			mkdir $$d ; \
		fi ; \
	done
	for d in $(SUBDIR) ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" INSTALL="$(INSTALL)" BIN="$(BIN)" LIB="$(LIB)" XLIBS="$(XLIBS)" INCLUDE="$(INCLUDE)" DA_OSTYPE="$(DA_OSTYPE)" install ) ; \
	done

clean:
	for d in $(SUBDIR) ; do \
		( cd $$d ; $(MAKE) RM="$(RM)" DA_OSTYPE="$(DA_OSTYPE)" clean ) ; \
	done
	$(RM) \#* *~

veryclean: clean
	for d in $(BIN) $(LIB) $(INCLUDE) ; do \
		if [ ! -d $$d ]; then \
			rm -rf $$d ; \
		fi ; \
	done
	$(RM) \#* *~

