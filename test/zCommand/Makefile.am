if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zCommandTest.zlog

check_PROGRAMS = zCommandUnitTest

zCommandUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zCommandTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Command.cpp

zCommandUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

