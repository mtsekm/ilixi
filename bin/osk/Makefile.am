## Makefile.am for bin/osk
bin_PROGRAMS 		= 	ilixi_osk
ilixi_osk_LDADD		=	$(top_srcdir)/$(PACKAGE)/lib$(PACKAGE)-$(VERSION).la  $(AM_LDFLAGS)
ilixi_osk_CPPFLAGS 	= 	-I$(top_srcdir)/$(PACKAGE) @DEPS_CFLAGS@
ilixi_osk_SOURCES	= 	OSK.cpp OSK.h TextInput.h TextInput.cpp