if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zProgramTest.zlog

check_PROGRAMS = zProgramUnitTest

zProgramUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zProgramTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Option.cpp \
    Argument.cpp \
    Program.cpp

zProgramUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

