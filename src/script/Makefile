#
# Makefile for Imai Kobayashi Lab. command
#

PREFIX	= /usr/local/SPTK
BIN	= $(PREFIX)/bin

RM	= rm -rf

.IGNORE:

all:

install:
	for i in *.sh ; do \
		sed '/path/s:/usr/local/SPTK/bin:'$(BIN)':' $$i > $(BIN)/$$i ; \
		chmod 755 $(BIN)/$$i ; \
		j=`basename $$i .sh` ; \
		$(RM) $$j ; \
		ln -s $(BIN)/$$i $(BIN)/$$j ; \
	done

clean:
	$(RM) \#* *~

veryclean: clean
	for i in *.sh ; do \
		j=`basename $$i .sh` ; \
		$(RM) $(BIN)/$$i $(BIN)/$$j ; \
	done

