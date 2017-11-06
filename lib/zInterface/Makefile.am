export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

SUBDIRS = \
    netlink \
    nl80211

if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS) -I$(srcdir) -I$(srcdir)/netlink -I$(srcdir)/nl80211
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzInterface.la

libzInterface_la_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

libzInterface_la_LDFLAGS = \
  ${AM_LIBS} \
  ${NL3_LIBS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

libzInterface_la_SOURCES = \
    InterfaceConfiguration.cpp \
    Interface.cpp \
    WirelessInterface.cpp \
    InterfaceFactory.cpp

libzInterface_la_LIBADD = \
    ${top_builddir}/lib/zInterface/netlink/libnetlink.la \
    ${top_builddir}/lib/zInterface/nl80211/libnl80211.la
