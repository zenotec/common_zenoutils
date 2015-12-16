if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zSocketUnitTest

zSocketUnitTest_SOURCES = \
    valgrind.sh \
    zSocketTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Buffer.cpp \
    Address.cpp \
    InetAddress.cpp \
    InetSocket.cpp

zSocketUnitTest_LDADD = \
    ${top_builddir}/src/libzutils.la

