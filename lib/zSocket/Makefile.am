export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

SUBDIRS = \
    ieee8023

if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

if COND_DEBUG
CPPFLAGS_DEBUG = -DDEBUG
endif

AM_CPPFLAGS := \
  $(CPPFLAGS_GCOV) \
  $(CPPFLAGS_DEBUG) \
  $(com_incdir) \
  $(BOOST_CPPFLAGS) \
  -I$(srcdir) \
  -I$(srcdir)/ieee8023

AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzSocket.la

libzSocket_la_SOURCES = \
    SocketBuffer.cpp \
    SocketAddress.cpp \
    Frame.cpp \
    SocketNotification.cpp \
    Socket.cpp \
    SocketAdapter.cpp \
    SocketHandler.cpp \
    SocketTap.cpp \
    LoopSocket.cpp \
    UnixSocket.cpp \
    RawSocket.cpp \
    InetSocket.cpp

libzSocket_la_LIBADD = \
    ${top_builddir}/lib/zSocket/ieee8023/libieee8023.la
    