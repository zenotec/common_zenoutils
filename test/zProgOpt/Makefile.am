if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zProgOptUnitTest

zProgOptUnitTest_SOURCES = \
    $(top_srcdir)/inc/UnitTest.h \
    valgrind.sh \
    zProgOptTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Option.cpp \
    Table.cpp

zProgOptUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

