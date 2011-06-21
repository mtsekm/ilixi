## Makefile.am for /ilixi/lib
INCLUDES 					= 	-I$(top_srcdir)/ilixi
noinst_LTLIBRARIES 			= 	libilixi_lib.la
libilixi_lib_la_CPPFLAGS 	= 	@DEPS_CFLAGS@
libilixi_lib_la_LIBADD 		= 	@DEPS_LIBS@

libilixi_lib_la_SOURCES 	= 	SQLITE3Driver.cpp \
								Animation.cpp \
								Easing.cpp \
								Tween.cpp \
								TweenAnimation.cpp 
								
ilixi_includedir			= 	$(includedir)/$(PACKAGE)-$(VERSION)/lib
ilixi_include_HEADERS		=	SQLITE3Driver.h \
								Animation.h \
								Easing.h \
								Tween.h \
								TweenAnimation.h 
