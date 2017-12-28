if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

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
  ${AM_LIBS} \
  ${NL3_LIBS} \
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
    Interface.cpp

zWirelessUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

