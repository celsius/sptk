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

