#
# Makefile for commands
#

TARGET		= psgr
SRCS		= $(TARGET).c plot.c eps.c dict.c
OBJS		= $(SRCS:.c=.o)

PREFIX		= /usr/local/SPTK
BIN		= $(PREFIX)/bin

CC		= gcc
RM		= rm -rf
INSTALL		= install -cs

INCDIR		= -I../../include
LIBDIR		= -L../../lib
CFLAGS		= -O2 $(INCDIR)
LDFLAGS		= $(LIBDIR)
LIBS		= -lSPTK -lX11 -lm

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

xgr.o: psgr.h
plot.o: psgr.h
eps.o: psgr.h

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)

clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)

