if COND_GCOV
GCOV_EXTRA_CPPFLAGS = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
GCOV_EXTRA_LDFLAGS = -lgcov
endif

AM_CPPFLAGS := $(GCOV_EXTRA_CPPFLAGS) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(GCOV_EXTRA_LDFLAGS)

check_PROGRAMS = zQueueUnitTest

zQueueUnitTest_SOURCES = \
    valgrind.sh \
    UnitTest.h \
    UnitTest.cpp \
    Defaults.cpp

zQueueUnitTest_LDADD = \
    ${top_builddir}/src/libzutils.la

