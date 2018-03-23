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
  -I${top_srcdir}/lib/zInterface/netlink

AM_LDFLAGS := $(LDFLAGS_GCOV)

NetlinkUnitTest_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

NetlinkUnitTest_LDFLAGS = \
  ${AM_LIBS} \
  ${NL3_LIBS} \
  ${AM_CPPFLAGS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

check_PROGRAMS = NetlinkUnitTest

NetlinkUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    NetlinkTest.h \
    UnitTest.cpp \
    Defaults.cpp

NetlinkUnitTest_LDADD = \
    ${top_builddir}/lib/zInterface/netlink/libnetlink.la \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zSem/libzSem.la
