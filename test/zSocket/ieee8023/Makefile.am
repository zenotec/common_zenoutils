if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
  $(CPPFLAGS_GCOV) \
  $(com_incdir) \
  $(BOOST_CPPFLAGS) \
  -I$(srcdir) \
  -I${top_srcdir}/test/inc \
  -I${top_srcdir}/lib/zSocket/ieee8023

AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = Ieee8023UnitTest

Ieee8023UnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    Ieee8023Test.h \
    UnitTest.cpp \
    Defaults.cpp

Ieee8023UnitTest_LDADD = \
    ${top_builddir}/lib/zSocket/libzSocket.la \
    ${top_builddir}/lib/zSocket/ieee8023/libieee8023.la \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zSem/libzSem.la
