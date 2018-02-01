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
  -I${top_srcdir}/lib/zInterface/netlink

AM_LDFLAGS := $(LDFLAGS_GCOV)

zInterfaceUnitTest_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

zInterfaceUnitTest_LDFLAGS = \
  ${AM_LDFLAGS} \
  ${NL3_LIBS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

CLEANFILES = \
	zInterfaceTest.zlog

check_PROGRAMS = zInterfaceUnitTest

zInterfaceUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zInterfaceTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Configuration.cpp \
    Interface.cpp \
    MacVlan.cpp \
    Factory.cpp

zInterfaceUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

