if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

SUBDIRS = \
	nl80211 \
	ieee80211

AM_CPPFLAGS := \
  $(CPPFLAGS_GCOV) \
  $(com_incdir) \
  $(BOOST_CPPFLAGS) \
  -I$(srcdir) \
  -I${top_srcdir}/test/inc \
  -I${top_srcdir}/lib/zInterface/netlink \
  -I${top_srcdir}/lib/zWireless/nl80211 \
  -I${top_srcdir}/lib/zWireless/ieee80211

AM_LDFLAGS := $(LDFLAGS_GCOV)

zWirelessUnitTest_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

zWirelessUnitTest_LDFLAGS = \
  ${AM_LDFLAGS} \
  ${NL3_LIBS} \
  ${AM_LDFLAGS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

CLEANFILES = \
	zWirelessTest.zlog

check_PROGRAMS = zWirelessUnitTest

zWirelessUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zWirelessTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Configuration.cpp \
    Interface.cpp \
    BasicServiceSet.cpp 
#    AccessPointInterface.cpp

zWirelessUnitTest_LDADD = \
    ${top_builddir}/lib/zWireless/libzWireless.la \
    ${top_builddir}/lib/zWireless/nl80211/libnl80211.la \
    ${top_builddir}/lib/zInterface/libzInterface.la \
    ${top_builddir}/lib/zInterface/netlink/libnetlink.la \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zConfig/libzConfig.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zData/libzData.la \
    ${top_builddir}/lib/zSem/libzSem.la

