if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zDisplayUnitTest

CLEANFILES = \
	zDisplayTest.zlog

zDisplayUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zDisplayTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    DisplayBuffer.cpp \
    DisplayVar.cpp \
    Display.cpp

zDisplayUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

