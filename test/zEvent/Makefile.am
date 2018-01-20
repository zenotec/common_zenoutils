if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zEventUnitTest

CLEANFILES = \
	zEventTest.zlog

zEventUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zEventTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Event.cpp \
    Handler.cpp \
    Manager.cpp

zEventUnitTest_LDADD = \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zSem/libzSem.la

