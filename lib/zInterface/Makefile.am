export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

SUBDIRS = \
    netlink

if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

if COND_DEBUG
CPPFLAGS_DEBUG = -DDEBUG
endif

AM_CPPFLAGS := \
	$(CPPFLAGS_GCOV) \
	$(com_incdir) \
	$(BOOST_CPPFLAGS) \
	$(CPPFLAGS_DEBUG) \
	-I$(srcdir) \
	-I$(srcdir)/netlink \
	-I$(srcdir)/nl80211

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
    Configuration.cpp \
    Interface.cpp \
    LoopInterface.cpp \
    MacVlanInterface.cpp

libzInterface_la_LIBADD = \
    ${top_builddir}/lib/zInterface/netlink/libnetlink.la
