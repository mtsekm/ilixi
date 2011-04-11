## Makefile.am for /ilixi/core
INCLUDES 					= 	-I$(top_srcdir)/ilixi
noinst_LTLIBRARIES 			= 	libilixi_core.la
libilixi_core_la_CPPFLAGS 	= 	@DEPS_CFLAGS@
libilixi_core_la_LIBADD 	= 	@DEPS_LIBS@

libilixi_core_la_SOURCES 	= 	AppBase.cpp \
								UIManager.cpp \
								Logger.cpp \
								Thread.cpp \
								Window.cpp
								
ilixi_includedir			= 	$(includedir)/$(PACKAGE)-$(VERSION)/core
ilixi_include_HEADERS		=	AppBase.h \
								UIManager.h \
								IFusion.h \
								Logger.h \
								Thread.h \
								Utils.h \
								Window.h
