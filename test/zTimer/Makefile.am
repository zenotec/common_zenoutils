if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zTimerTest.zlog

check_PROGRAMS = zTimerUnitTest

zTimerUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zTimerTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    StartStop.cpp

zTimerUnitTest_LDADD = \
    ${top_builddir}/lib/zTimer/libzTimer.la \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zSem/libzSem.la

