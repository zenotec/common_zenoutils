if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) -I${top_srcdir}/test/inc $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

check_PROGRAMS = zDataUnitTest

CLEANFILES = \
	zDataTest.zlog

zDataUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zDataTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Path.cpp \
    PutValue.cpp \
 	Child.cpp \
 	Array.cpp \
 	Json.cpp \
 	Xml.cpp \
 	Copy.cpp

zDataUnitTest_LDADD = \
    ${top_builddir}/lib/libzutils.la

