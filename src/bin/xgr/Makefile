#
# Makefile for commands
#

TARGET		= xgr
SRCS		= $(TARGET).c plot.c window.c
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
LIBS		= -lSPTK -lm
XLIBS		= -lX11

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS) $(XLIBS)

xgr.o: xgr.h
plot.o: xgr.h gcdata.h
window.o: xgr.h gcdata.h

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)

clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)

