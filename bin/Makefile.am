## Makefile.am for bin/
SUBDIRS = osk home monitor statusbar widget_gallery

miscdatadir 			=	$(ILIXI_DATADIR)
miscdata_DATA 			=	maestro.conf
EXTRA_DIST 				= 	$(miscdata_DATA)
dist_noinst_SCRIPTS 	= 	setup.sh

bin_PROGRAMS 			= 	ilixi_maestro
ilixi_maestro_LDADD		=	$(top_srcdir)/$(PACKAGE)/lib$(PACKAGE)-$(VERSION).la  $(AM_LDFLAGS)
ilixi_maestro_CPPFLAGS 	= 	-I$(top_srcdir)/$(PACKAGE) @DEPS_CFLAGS@
ilixi_maestro_SOURCES	= 	Maestro.cpp