#
# Makefile for commands
#

TARGET		= ds
SRCS		= $(TARGET).c
OBJS		= $(SRCS:.c=.o)
COEF		= lpfcoef.2to1 lpfcoef.4to3 lpfcoef.5to2up lpfcoef.5to2dn

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
	$(CC) -DLIB=\"$(LIB)\" $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)
	cp $(COEF) $(LIB)

clean:
	$(RM) $(TARGET) $(OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)
	cd $(LIB) ; $(RM) $(COEF)

