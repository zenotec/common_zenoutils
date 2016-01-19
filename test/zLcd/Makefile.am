if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zLcdUnitTest

zLcdUnitTest_SOURCES = \
    valgrind.sh \
    zLcdTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    LcdVar.cpp \
    Lcd.cpp

zLcdUnitTest_LDADD = \
    ${top_builddir}/src/libzutils.la

