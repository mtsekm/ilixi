## Makefile.am for bin/
SUBDIRS 		= 	osk home statusbar

miscdatadir 		=	$(ILIXI_DATADIR)
miscdata_DATA 		=	maestro.conf ilixi.db
EXTRA_DIST 		= 	$(miscdata_DATA)

bin_PROGRAMS 		= 	ilixi_maestro
ilixi_maestro_LDADD	=	$(top_srcdir)/$(PACKAGE)/lib$(PACKAGE)-$(VERSION).la  $(AM_LDFLAGS)
ilixi_maestro_CPPFLAGS 	= 	-I$(top_srcdir)/$(PACKAGE) @DEPS_CFLAGS@
ilixi_maestro_SOURCES	= 	Maestro.cpp


