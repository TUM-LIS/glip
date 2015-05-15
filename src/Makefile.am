SUBDIRS = . tools

pkginclude_HEADERS = $(top_srcdir)/include/libglip.h

lib_LTLIBRARIES = libglip.la

libglip_la_SOURCES =\
	cbuf.c \
	glip.c \
	util.c

if BACKEND_CYPRESSFX2
libglip_la_SOURCES += backend_cypressfx2/sw/backend_cypressfx2.c
AM_CFLAGS += $(libusb_CFLAGS)
AM_LDFLAGS += $(libusb_LIBS)
endif

if BACKEND_TCP
libglip_la_SOURCES += backend_tcp/sw/backend_tcp.c
endif

libglip_la_LDFLAGS = $(AM_LDFLAGS) $(LTLDFLAGS)
libglip_la_CPPFLAGS = $(AM_CPPFLAGS) \
                      -I $(top_srcdir)/include
