if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zGpioUnitTest

#zGpioUnitTest_SOURCES = \
#    valgrind.sh \
#    zGpioTest.h \
#    UnitTest.cpp \
#    Defaults.cpp \
#    Port.cpp \
#    Handler.cpp \
#    Switch.cpp

zGpioUnitTest_SOURCES = \
    valgrind.sh \
    zGpioTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Configuration.cpp

zGpioUnitTest_LDADD = \
    ${top_builddir}/src/libzutils.la

