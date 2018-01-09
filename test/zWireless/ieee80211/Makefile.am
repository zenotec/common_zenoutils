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

check_PROGRAMS = Ieee80211UnitTest

Ieee80211UnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    Ieee80211Test.h \
    UnitTest.cpp \
    Defaults.cpp \
    Radiotap.cpp \
    Frame.cpp \
    ManagementFrame.cpp \
    Beacon.cpp \
    Probe.cpp \
    Association.cpp \
    Authentication.cpp

Ieee80211UnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

