if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zWirelessTest.zlog

check_PROGRAMS = zWirelessUnitTest

zWirelessUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zWirelessTest.h \
    UnitTest.cpp \
    Defaults.cpp

zWirelessUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la
