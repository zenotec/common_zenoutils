if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zComUnitTest

zComUnitTest_SOURCES = \
    valgrind.sh \
    zComTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Port.cpp \
    TtyPort.cpp

zComUnitTest_LDADD = \
    ${top_builddir}/src/libzutils.la

