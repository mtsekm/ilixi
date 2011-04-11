## Makefile.am for bin/widget_gallery
bin_PROGRAMS 			= 	ilixi_gallery
ilixi_gallery_LDADD		=	$(top_srcdir)/$(PACKAGE)/lib$(PACKAGE)-$(VERSION).la $(AM_LDFLAGS)
ilixi_gallery_CPPFLAGS 	= 	-I$(top_srcdir)/$(PACKAGE) @DEPS_CFLAGS@
ilixi_gallery_SOURCES	= 	WidgetGallery.cpp WidgetGallery.h
miscdatadir 			=	$(ILIXI_DATADIR)
miscdata_DATA 			= 	gallery.png						
EXTRA_DIST 				= 	$(miscdata_DATA)
