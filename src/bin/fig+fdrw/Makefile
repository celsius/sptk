#
# Makefile for commands
#

TARGET		= fdrw fig
FDRW_SRCS	= fdrw.c
FIG_SRCS	= fig.c fig0.c fig1.c
SRCS		= plot.c plsub.c
FDRW_OBJS	= $(FDRW_SRCS:.c=.o)
FIG_OBJS	= $(FIG_SRCS:.c=.o)
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


.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(TARGET)

fdrw: $(FDRW_OBJS) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(FDRW_OBJS) $(OBJS) $(LIBS)

fig: $(FIG_OBJS) $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(FIG_OBJS) $(OBJS) $(LIBS)

fig0.o: fig.h
fig1.o: fig.h
plot.o: plot.h
plsub.o: plot.h

install: $(TARGET)
	$(INSTALL) $(TARGET) $(BIN)

clean:
	$(RM) $(TARGET) $(OBJS) $(FIG_OBJS) $(FDRW_OBJS) \#* *~

veryclean: clean
	$(RM) $(BIN)/$(TARGET)

