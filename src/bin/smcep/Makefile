#
# Makefile for commands
#

TARGET          = smcep
SRCS		= _$(TARGET).c $(TARGET).c
OBJS            = $(SRCS:.c=.o)

PREFIX          = /usr/local/cmnd
BIN		= $(HOME)/bin/$(OSNAME)

CC_OSF1		= cc
CC_HP-UX	= gcc
CC_Solaris	= gcc
CC_SunOS	= gcc
CC		= $(CC_$(OSNAME))

RM		= rm -f
INSTALL		= cp -i

INCDIR		= -I$(PREFIX)/include
LIBDIR		= -L$(PREFIX)/lib
CFLAGS          = -O2 $(INCDIR)
LDFLAGS		= $(LIBDIR)
LIBS            = -lSPTK -lm



all:$(TARGET)


.c.o:
	$(CC) $(CFLAGS) -c $<


$(TARGET):$(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)


install:$(TARGET)
	$(INSTALL) $(TARGET) $(BIN)


clean:
	$(RM) $(OBJS) $(TARGET)


veryclean:clean
	$(RM) -i $(BIN)/$(TARGET)
