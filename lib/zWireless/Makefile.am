export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

SUBDIRS = \
    nl80211 \
    ieee80211 \
    ieee8023

if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

if COND_DEBUG
CPPFLAGS_DEBUG = -DDEBUG
endif

AM_CPPFLAGS := \
  $(CPPFLAGS_GCOV) \
  $(CPPFLAGS_DEBUG) \
  $(com_incdir) \
  $(BOOST_CPPFLAGS) \
  -I$(srcdir) \
  -I${top_srcdir}/lib/zInterface/netlink \
  -I$(srcdir)/nl80211 \
  -I$(srcdir)/ieee80211 \
  -I$(srcdir)/ieee8023

AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzWireless.la

libzWireless_la_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

libzWireless_la_LDFLAGS = \
  ${AM_LIBS} \
  ${NL3_LIBS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

libzWireless_la_SOURCES = \
    Configuration.cpp \
    Interface.cpp \
    WirelessSocket.cpp \
    AccessPointInterface.cpp \
    BasicServiceSet.cpp

libzWireless_la_LIBADD = \
    ${top_builddir}/lib/zWireless/nl80211/libnl80211.la \
    ${top_builddir}/lib/zWireless/ieee80211/libieee80211.la \
    ${top_builddir}/lib/zWireless/ieee8023/libieee8023.la
    