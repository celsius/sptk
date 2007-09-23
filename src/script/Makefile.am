## Process this file with automake to produce Makefile.in
##
## $Id: Makefile.am,v 1.4 2007/09/23 02:46:43 heigazen Exp $
bin_SCRIPTS = bell.sh glogsp.sh grlogsp.sh gwave.sh
EXTRA_DIST = bell.in glogsp.in grlogsp.in gwave.in

CLEANFILES = $(bin_SCRIPTS)

replace_path  = sed -e 's,@binpath@,$(DESTDIR)$(bindir),g' \
	       -e 's,@libpath@,$(DESTDIR)$(libdir),g' \
	       -e 's,@sptk_version@,$(PACKAGE_VERSION),g' \
	       -e 's,@cshpath@,$(CSH),' \
	       -e 's,@use_double@,$(use_double),g'

.in.sh: 
	$(replace_path) < $< > $@
	chmod +x $@

install-binSCRIPTS: $(bin_SCRIPTS)
	@$(NORMAL_INSTALL)
	test -z "$(bindir)" || $(mkdir_p) "$(DESTDIR)$(bindir)"
	@list='$(bin_SCRIPTS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  if test -f $$d$$p; then \
	    f=`echo "$$p" | sed 's|^.*/||;$(transform)'`; \
	    echo " $(binSCRIPT_INSTALL) '$$d$$p' '$(DESTDIR)$(bindir)/$$f'"; \
	    $(binSCRIPT_INSTALL) "$$d$$p" "$(DESTDIR)$(bindir)/$$f"; \
	  else :; fi; \
	  p2=`basename $$p .sh`; \
	  rm -f "$(DESTDIR)$(bindir)/$$p2"; \
	  (cd $(DESTDIR)$(bindir); $(LN_S) $$p  $$p2) \
	done


uninstall-binSCRIPTS:
	@$(NORMAL_UNINSTALL)
	@list='$(bin_SCRIPTS)'; for p in $$list; do \
	  f=`echo "$$p" | sed 's|^.*/||;$(transform)'`; \
	  echo " rm -f '$(DESTDIR)$(bindir)/$$f'"; \
	  rm -f "$(DESTDIR)$(bindir)/$$f"; \
	  p2=`basename $$p .sh`; \
	  rm -f "$(DESTDIR)$(bindir)/$$p2"; \
	done
