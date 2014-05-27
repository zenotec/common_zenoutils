
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
                      zLog.cpp \
                      zThread.cpp \
                      zSemaphore.cpp \
                      zEvent.cpp \
                      zTimer.cpp \
                      zData.cpp \
                      zNode.cpp \
                      zSocket/SocketBuffer.cpp \
                      zSocket/SocketAddress.cpp \
                      zSocket/Socket.cpp 

