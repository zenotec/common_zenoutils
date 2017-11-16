if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zSerialTest.zlog

check_PROGRAMS = zSerialUnitTest

zSerialUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zSerialTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Config.cpp \
    Port.cpp \
    TtyPort.cpp

zSerialUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

