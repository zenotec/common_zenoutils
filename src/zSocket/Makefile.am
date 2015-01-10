if COND_GCOV
GCOV_EXTRA_CPPFLAGS = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
GCOV_EXTRA_LDFLAGS = -lgcov
endif

AM_CPPFLAGS := $(GCOV_EXTRA_CPPFLAGS) $(com_incdir) $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(GCOV_EXTRA_LDFLAGS)

noinst_LTLIBRARIES = libzSocket.la

libzSocket_la_SOURCES = \
    SocketBuffer.cpp \
    SocketAddress.cpp \
    Socket.cpp

