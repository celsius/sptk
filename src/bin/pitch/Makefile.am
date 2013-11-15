## Process this file with automake to produce Makefile.in
##
## $Id$
AM_CPPFLAGS = -I$(top_srcdir)/include
bin_PROGRAMS = pitch
pitch_SOURCES = pitch.c $(srcdir)/snack/jkGetF0.c $(srcdir)/snack/sigproc.c $(srcdir)/snack/jkGetF0.h \
	$(srcdir)/swipe/swipe.c $(srcdir)/swipe/vector.c $(srcdir)/swipe/vector.h
pitch_LDADD = $(top_builddir)/lib/libSPTK.a
