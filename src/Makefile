########################################################################
#
# Makefile for Speech Signal Processing Commands
#
########################################################################
#
# Location to install programs, libraries, and include files.
#
PREFIX		= /usr/local/SPTK
BIN		= $(PREFIX)/bin
LIB		= $(PREFIX)/lib
INCLUDE		= $(PREFIX)/include

#
# Directories which contain libraries and include files of X11.
#
XINCDIR		= /usr/local/X11R6.3/include
XLIBDIR		= /usr/local/X11R6.3/lib
#XLIBS		= -lX11
XLIBS		= -lX11 -lsocket

#
# Compiler and Instllation programs.
# Some systems should replace 'install -cs' with 'cp'.
#
CC		= gcc -O2
RM		= rm -rf
INSTALL		= install -cs

#
# Uncomment if you want to deal with data in double
#
#DOUBLE		= -DDOUBLE

########################################################################

SUBDIR		= lib include bin script

INCDIR		= -I../include -I../../include -I$(XINCDIR)
LIBDIR		= -L../../lib -L$(XLIBDIR)
CFLAGS		= $(INCDIR) $(DOUBLE)
LDFLAGS		= $(LIBDIR)
LIBS		= -lSPTK -lm


all:
	for d in $(SUBDIR) ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" LIB="$(LIB)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" XLIBS="$(XLIBS)" ) ; \
	done

install:
	for d in $(PREFIX) $(BIN) $(LIB) $(INCLUDE) ; do \
		if [ ! -d $$d ]; then \
			rm -rf $$d ; \
			mkdir $$d ; \
		fi ; \
	done
	for d in $(SUBDIR) ; do \
		( cd $$d ; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)" LIBS="$(LIBS)" INSTALL="$(INSTALL)" BIN="$(BIN)" LIB="$(LIB)" XLIBS="$(XLIBS)" INCLUDE="$(INCLUDE)" install ) ; \
	done

clean:
	for d in $(SUBDIR) ; do \
		( cd $$d ; $(MAKE) RM="$(RM)" clean ) ; \
	done
	$(RM) \#* *~

veryclean: clean
#	for d in $(SUBDIR) ; do \
#		( cd $$d ; $(MAKE) RM="$(RM)" veryclean ) ; \
#	done
	for d in $(BIN) $(LIB) $(INCLUDE) ; do \
		if [ ! -d $$d ]; then \
			rm -rf $$d ; \
		fi ; \
	done
	$(RM) \#* *~

