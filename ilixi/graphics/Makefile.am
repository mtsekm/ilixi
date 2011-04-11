## Makefile.am for /ilixi/graphics
INCLUDES						= 	-I$(top_srcdir)/ilixi
noinst_LTLIBRARIES 				= 	libilixi_graphics.la
libilixi_graphics_la_CPPFLAGS 	= 	@DEPS_CFLAGS@
libilixi_graphics_la_LIBADD 	= 	@DEPS_LIBS@

libilixi_graphics_la_SOURCES 	= 	Designer.cpp \
									Painter.cpp \
									Palette.cpp \
									Surface.cpp \
									TDesigner.cpp
									
ilixi_includedir 				= 	$(includedir)/$(PACKAGE)-$(VERSION)/graphics
nobase_ilixi_include_HEADERS 	= 	Designer.h \
									Painter.h \
									Palette.h \
									Surface.h \
									TDesigner.h
									
