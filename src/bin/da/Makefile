#
# Makefile for commands
#

TARGET		= dawrite
SRCS		= $(TARGET).c
OBJS		= $(SRCS:.c=.o)

PREFIX		= /usr/local/SPTK
BIN		= $(PREFIX)/bin
LIB		= $(PREFIX)/lib

CC		= gcc
RM		= rm -rf
INSTALL		= install -cs

INCDIR		= -I../../include
LIBDIR		= -L../../lib
CFLAGS		= -O2 $(INCDIR)
LDFLAGS		= $(LIBDIR)
LIBS		= -lSPTK -lm

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
		$(RM) $$j ; \
		ln -s $(BIN)/$$i $(BIN)/$$j ; \
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

