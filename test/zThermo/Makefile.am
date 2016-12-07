if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zThermoUnitTest

zThermoUnitTest_SOURCES = \
    valgrind.sh \
    zThermoTest.h \
    UnitTest.cpp \
    Defaults.cpp

zThermoUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

