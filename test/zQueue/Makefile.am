if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zQueueUnitTest

zQueueUnitTest_SOURCES = \
    valgrind.sh \
    UnitTest.h \
    UnitTest.cpp \
    Defaults.cpp

zQueueUnitTest_LDADD = \
    ${top_builddir}/src/libzutils.la

