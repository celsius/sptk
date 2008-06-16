# Makefile for VC++ 2005 nmake tool
#
#            2007.10  Jong-Jin Kim
# 

# You can modify this variable's value to your own
INSTALLDIR = c:\SPTK-3.2.win\bin

all:
	cd lib
	nmake /f Makefile.mak
	cd ..
	
	cd bin
	nmake /f Makefile.mak
	cd ..

clean:
	cd lib
	nmake /f Makefile.mak clean
	cd ..
	
	cd bin
	nmake /f Makefile.mak clean
	cd ..
	
install::
	@if not exist "$(INSTALLDIR)" mkdir "$(INSTALLDIR)"
	
	cd bin
	copy *.exe $(INSTALLDIR)
	cd ..
