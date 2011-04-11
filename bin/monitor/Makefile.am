## Makefile.am for bin/monitor
bin_PROGRAMS 			= 	ilixi_monitor
ilixi_monitor_LDADD		=	$(top_srcdir)/$(PACKAGE)/lib$(PACKAGE)-$(VERSION).la  $(AM_LDFLAGS)
ilixi_monitor_CPPFLAGS 	= 	-I$(top_srcdir)/$(PACKAGE) @DEPS_CFLAGS@
ilixi_monitor_SOURCES	= 	CPUMonitor.h \
							CPUMonitor.cpp \
							FSMonitor.h \
							FSMonitor.cpp \
							MEMMonitor.h \
							MEMMonitor.cpp \
							Monitor.cpp \
							Monitor.h \
							NETMonitor.h \
							NETMonitor.cpp \
							OSMonitor.h \
							OSMonitor.cpp
miscdatadir 			=	$(ILIXI_DATADIR)
miscdata_DATA 			=	monitor.png						
EXTRA_DIST 				= 	$(miscdata_DATA)
							