#
# Makefile for commands
#

TARGET		= da
SRCS		= $(TARGET).c
OBJS		= $(SRCS:.c=.o)
COEF		= da_10.fir da_12.fir

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

da.o: ss10snd.h

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)
	cp $(COEF) $(LIB)

clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)
	cd $(LIB) ; $(RM) $(COEF)
endif
all:
clean:

