
AM_CPPFLAGS = -I$(top_srcdir)/inc $(BOOST_CPPFLAGS)

lib_LTLIBRARIES = libzutil.la

# Where to install the headers on the system
libzutil_ladir = $(includedir)/zutils

libzutil_la_HEADERS = \
                    $(top_srcdir)/inc/zutils/zLog.h \
                    $(top_srcdir)/inc/zutils/zThread.h \
                    $(top_srcdir)/inc/zutils/zSemaphore.h \
                    $(top_srcdir)/inc/zutils/zQueue.h \
                    $(top_srcdir)/inc/zutils/zEvent.h \
                    $(top_srcdir)/inc/zutils/zTimer.h \
                    $(top_srcdir)/inc/zutils/zData.h \
                    $(top_srcdir)/inc/zutils/zSocket.h \
                    $(top_srcdir)/inc/zutils/zNode.h


libzutil_la_SOURCES = $(libzutil_a_HEADERS) \
                      $(top_srcdir)/src/zLog.cpp \
                      $(top_srcdir)/src/zThread.cpp \
                      $(top_srcdir)/src/zSemaphore.cpp \
                      $(top_srcdir)/src/zEvent.cpp \
                      $(top_srcdir)/src/zTimer.cpp \
                      $(top_srcdir)/src/zData.cpp \
                      $(top_srcdir)/src/zNode.cpp \
                      $(top_srcdir)/src/zSocket/SocketBuffer.cpp \
                      $(top_srcdir)/src/zSocket/SocketAddress.cpp \
                      $(top_srcdir)/src/zSocket/Socket.cpp 

