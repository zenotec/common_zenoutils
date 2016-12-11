if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zConfigUnitTest

zConfigUnitTest_SOURCES = \
    valgrind.sh \
    zConfTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Data.cpp \
    Configuration.cpp \
    File.cpp

zConfigUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

