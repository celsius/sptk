#!/bin/tcsh -f

#onintr CLEAR

set TODAY	= `date +%y%m%d`
set BASEDIR	= /usr/local/SPTK
set MKDISTDIR	= $BASEDIR/makedist
set SRCDIR	= $BASEDIR/src
set SNAPDIR	= $BASEDIR/snapshot
set LIST	= $MKDISTDIR/list.all
set ARCHIVE	= SPTK-snap-$TODAY
set DOCUMENT	= SPTK-doc-snap-$TODAY

cd $SNAPDIR

if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz{,.old}
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz{,.old}

rm -rf $ARCHIVE $DOCUMENT

make -f $MKDISTDIR/Makefile TARGETDIR=$ARCHIVE `cat $LIST` \
  && tar cfz $ARCHIVE.tar.gz $ARCHIVE \
  && rm -rf $ARCHIVE

$MKDISTDIR/cp-tex.pl -v -l $LIST -d $DOCUMENT \
  && tar cfz $DOCUMENT.tar.gz $DOCUMENT \
  && rm -rf $DOCUMENT

goto END

CLEAR:
rm -rf $ARCHIVE $DOCUMENT

END:

