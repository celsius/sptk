#
# Makefile for commands
#

TARGET		= libSPTK.a

PREFIX		= /usr/local/SPTK
LIB		= $(PREFIX)/lib

CC		= gcc
RM		= rm -rf

INCDIR		= -I../include
CFLAGS		= -O2 $(INCDIR)


.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(TARGET)

$(TARGET):
	$(MAKE) -f Makefile.lib PREFIX="$(PREFIX)" LIB="$(LIB)" CC="$(CC)" CFLAGS="$(CFLAGS)" CMNDSRCS="`echo ../bin/*/_*.c | sed 's:\.\./bin/[0-9_a-z]*/::g'`"

install: $(TARGET)
	cp $(TARGET) $(LIB)
	ranlib $(LIB)/$(TARGET)

clean:
	$(RM) $(TARGET) *.o \#* *~

veryclean: clean
	$(RM) $(LIB)/$(TARGET)

