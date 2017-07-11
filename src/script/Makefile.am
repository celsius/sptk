bin_SCRIPTS = bell gseries glogsp grlogsp gwave mgc2mgclsp mgclsp2mgc raw2wav wav2raw
EXTRA_DIST = bell.in gseries.in glogsp.in grlogsp.in gwave.in mgc2mgclsp.in mgclsp2mgc.in raw2wav.in wav2raw.in

CLEANFILES = $(bin_SCRIPTS)

replace_path = sed \
               -e 's,[@]bindir[@],$(bindir),g' \
               -e 's,[@]package_version[@],$(PACKAGE_VERSION),g' \
               -e 's,[@]csh[@],$(CSH),' \
               -e 's,[@]io_symbol[@],$(IO_SYMBOL),g' \
               -e 's,[@]io_format[@],$(IO_FORMAT),g'

bell: @srcdir@/bell.in
	$(replace_path) < "$<" > $@
	chmod +x $@

gseries: @srcdir@/gseries.in
	$(replace_path) < "$<" > $@
	chmod +x $@

glogsp: @srcdir@/glogsp.in
	$(replace_path) < "$<" > $@
	chmod +x $@

grlogsp: @srcdir@/grlogsp.in
	$(replace_path) < "$<" > $@
	chmod +x $@

gwave: @srcdir@/gwave.in
	$(replace_path) < "$<" > $@
	chmod +x $@

mgc2mgclsp: @srcdir@/mgc2mgclsp.in
	$(replace_path) < "$<" > $@
	chmod +x $@

mgclsp2mgc: @srcdir@/mgclsp2mgc.in
	$(replace_path) < "$<" > $@
	chmod +x $@

raw2wav: @srcdir@/raw2wav.in
	$(replace_path) < "$<" > $@
	chmod +x $@

wav2raw: @srcdir@/wav2raw.in
	$(replace_path) < "$<" > $@
	chmod +x $@
