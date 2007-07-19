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
# Makefile for commands
#

TARGET = dawrite
SRCS = $(TARGET).c
OBJS = $(SRCS:.c=.o)

PREFIX = /usr/local/SPTK
BIN = $(PREFIX)/bin
LIB = $(PREFIX)/lib

CC	= gcc
RM	= rm -rf
INSTALL = install -cs

INCDIR = -I../../include
LIBDIR = -L../../lib
CFLAGS = -O2 -ansi -Wall -DSPTK_VERSION=\"3.1\" $(INCDIR)
LDFLAGS = $(LIBDIR)
LIBS = -lSPTK -lm

.c.o:
	if [ x"$(DA_OSTYPE)" != x ] ; then \
	$(CC) -DLIB=\"$(LIB)\" $(CFLAGS) -D$(DA_OSTYPE) -c $< ; \
	fi

all: $(TARGET)

$(TARGET): $(OBJS)
	if [ x"$(DA_OSTYPE)" != x ] ; then \
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) ; \
	fi

dawrite.o: da.h

install: $(TARGET)
	if [ x"$(DA_OSTYPE)" != x ] ; then \
	$(INSTALL) $(TARGET) $(BIN) ; \
	for i in *.sh ; do \
		sed -e '/path/s:/usr/local/SPTK/bin:'$(BIN)':' -e '/libpath/s:/usr/local/SPTK/lib:'$(LIB)':' $$i > $(BIN)/$$i ; \
		chmod 755 $(BIN)/$$i ; \
		j=`basename $$i .sh` ; \
		$(RM) $(BIN)/$$j ; \
		ln -s $$i $(BIN)/$$j ; \
	done ; \
	fi

clean:
	if [ x"$(DA_OSTYPE)" != x ] ; then \
	$(RM) $(TARGET) $(OBJS) \#* *~ ; \
	fi

veryclean: clean
	if [ x"$(DA_OSTYPE)" != x ] ; then \
	$(RM) $(BIN)/$(TARGET) ; \
	for i in *.sh ; do \
		j=`basename $$i .sh` ; \
		$(RM) $(BIN)/$$i $(BIN)/$$j ; \
	done ; \
	fi

