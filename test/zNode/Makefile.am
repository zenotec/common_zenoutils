if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	zNodeTest.zlog

check_PROGRAMS = zNodeUnitTest

zNodeUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zNodeTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Node.cpp \
    Table.cpp

zNodeUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

