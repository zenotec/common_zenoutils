
AM_CPPFLAGS = -I$(top_srcdir)/inc $(BOOST_CPPFLAGS)

lib_LTLIBRARIES = libzutil.la

# Where to install the headers on the system
libzutil_ladir = $(includedir)/zutils

libzutil_la_HEADERS = $(top_srcdir)/inc/zData.h \
                      $(top_srcdir)/inc/zEvent.h \
                      $(top_srcdir)/inc/zSemaphore.h \
                      $(top_srcdir)/inc/zTimer.h \
                      $(top_srcdir)/inc/zSelect.h \
                      $(top_srcdir)/inc/zNode.h

libzutil_la_SOURCES = $(libzutil_a_HEADERS) \
                      $(top_srcdir)/src/zData.cpp \
                      $(top_srcdir)/src/zEvent.cpp \
                      $(top_srcdir)/src/zSemaphore.cpp \
                      $(top_srcdir)/src/zTimer.cpp \
                      $(top_srcdir)/src/zSelect.cpp \
                      $(top_srcdir)/src/zNode.cpp 

