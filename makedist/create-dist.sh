#!/bin/tcsh -f

#onintr CLEAR

set TODAY	= `date +%y%m%d`
set BASEDIR	= ..
set MKDISTDIR	= .
set ARCHIVEDIR	= $MKDISTDIR/release
set LIST	= $MKDISTDIR/list.release
set TEST	= 1

set VERSION	= `cat $MKDISTDIR/VERSION`
set MAJ_VER	= $VERSION:r
set MIN_VER	= $VERSION:e
if ( x"$MIN_VER" == x ) then
	set MIN_VER = 0
else
	@ MIN_VER++
endif
set VERSION = $MAJ_VER.$MIN_VER

set ARCHIVE	= SPTK-$VERSION
set DOCUMENT	= SPTKref-$VERSION

if ( ! -d $ARCHIVEDIR ) then
	rm -rf $ARCHIVEDIR
	mkdir -p $ARCHIVEDIR
endif

if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz{,.old}
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz{,.old}

rm -rf $ARCHIVE $DOCUMENT

make -f $MKDISTDIR/Makefile SOURCEDIR=$BASEDIR TARGETDIR=$ARCHIVE `cat $LIST` \
  && tar cfz $ARCHIVE.tar.gz $ARCHIVE \
  && rm -rf $ARCHIVE
if ( -f $ARCHIVE.tar.gz ) mv $ARCHIVE.tar.gz $ARCHIVEDIR

$MKDISTDIR/cp-tex.pl -b $BASEDIR -v -l $LIST -d $DOCUMENT \
  && tar cfz $DOCUMENT.tar.gz $DOCUMENT \
  && rm -rf $DOCUMENT
if ( -f $DOCUMENT.tar.gz ) mv $DOCUMENT.tar.gz $ARCHIVEDIR

goto END

CLEAR:
rm -rf $ARCHIVE $DOCUMENT

END:
if ( $TEST == 0 ) then
	echo $VERSION >! $MKDISTDIR/VERSION
endif

