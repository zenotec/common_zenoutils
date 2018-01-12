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
  -I${top_srcdir}/lib/zInterface/netlink \
  -I${top_srcdir}/lib/zWireless/nl80211 \
  -I${top_srcdir}/lib/zWireless/ieee80211

AM_LDFLAGS := $(LDFLAGS_GCOV)

Nl80211UnitTest_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

Nl80211UnitTest_LDFLAGS = \
  ${AM_LIBS} \
  ${NL3_LIBS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

check_PROGRAMS = Nl80211UnitTest

Nl80211UnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    Nl80211Test.h \
    UnitTest.cpp \
    Defaults.cpp

Nl80211UnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

