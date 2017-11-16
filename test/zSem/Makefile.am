if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zSemTest.zlog

check_PROGRAMS = zSemUnitTest

zSemUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zSemTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Mutex.cpp \
    Semaphore.cpp

zSemUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

