#
# Makefile for Imai Kobayashi Lab. command
#

PREFIX		= /usr/local/SPTK
BIN		= $(PREFIX)/bin
LIB		= $(PREFIX)/lib

CC		= gcc
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
		( cd $$d ; $(MAKE) CC="$(CC)" LIB="$(LIB)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" XLIBS="$(XLIBS)" ) ; \
	done

install:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" XLIBS="$(XLIBS)" INSTALL="$(INSTALL)" BIN="$(BIN)" install ) ; \
	done

clean:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) RM="$(RM)" clean ) ; \
	done
	$(RM) \#* *~

veryclean:
	for d in [a-z]* ; do \
		( cd $$d ; $(MAKE) RM="$(RM)" veryclean ) ; \
	done
	$(RM) \#* *~

