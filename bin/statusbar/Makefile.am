## Makefile.am for bin/statusbar
bin_PROGRAMS 				= 	ilixi_statusbar
ilixi_statusbar_LDADD		=	$(top_srcdir)/$(PACKAGE)/lib$(PACKAGE)-$(VERSION).la  $(AM_LDFLAGS)
ilixi_statusbar_CPPFLAGS 	= 	-I$(top_srcdir)/$(PACKAGE) @DEPS_CFLAGS@
ilixi_statusbar_SOURCES		= 	SDialog.h SDialog.cpp StatusBar.cpp StatusBar.h