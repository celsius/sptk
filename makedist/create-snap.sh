#!/bin/tcsh -f

#onintr CLEAR

set TODAY	= `date +%y%m%d`
set BASEDIR	= ..
set MKDISTDIR	= .
set SNAPDIR	= $MKDISTDIR/snapshot
set LIST	= $MKDISTDIR/list.all
set ARCHIVE	= SPTK-snap-$TODAY
set DOCUMENT	= SPTK-doc-snap-$TODAY

if ( ! -d $SNAPDIR ) then
	rm -rf $SNAPDIR
	mkdir -p $SNAPDIR
endif

if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz{,.old}
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz{,.old}

rm -rf $ARCHIVE $DOCUMENT

make -f $MKDISTDIR/Makefile SOURCEDIR=$BASEDIR TARGETDIR=$ARCHIVE `cat $LIST` \
  && tar cfz $ARCHIVE.tar.gz $ARCHIVE \
  && rm -rf $ARCHIVE
if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz $SNAPDIR

$MKDISTDIR/cp-tex.pl -v -l $LIST -d $DOCUMENT \
  && tar cfz $DOCUMENT.tar.gz $DOCUMENT \
  && rm -rf $DOCUMENT
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz $SNAPDIR

goto END

CLEAR:
rm -rf $ARCHIVE $DOCUMENT

END:

