if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zMessageTest.zlog

check_PROGRAMS = zMessageUnitTest

zMessageUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zMessageTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Message.cpp \
    Factory.cpp \
    MessageSocket.cpp

zMessageUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

