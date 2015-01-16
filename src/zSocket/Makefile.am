if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzSocket.la

libzSocket_la_SOURCES = \
    Buffer.cpp \
    Address.cpp \
    Socket.cpp \
    Handler.cpp \
    InetAddress.cpp \
    InetSocket.cpp

