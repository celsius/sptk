#
# Makefile for commands
#

TARGET		= da
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

ifdef DA_OSTYPE
.c.o:
	$(CC) -DLIB=\"$(LIB)\" $(CFLAGS) -D$(DA_OSTYPE) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

da.o: da.h

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)
	for i in *.sh ; do \
		sed '/path/s:/usr/local/SPTK/bin:'$(BIN)':' $$i > $(BIN)/$$i ; \
		chmod 755 $(BIN)/$$i ; \
		j=`basename $$i .sh` ; \
		$(RM) $$j ; \
		ln -s $(BIN)/$$i $(BIN)/$$j ; \
	done

clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)
	cd $(LIB) ; $(RM)
endif
all:
clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

