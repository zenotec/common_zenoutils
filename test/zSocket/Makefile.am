if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
	$(CPPFLAGS_GCOV) \
	$(com_incdir) \
	-I${top_srcdir}/test/inc \
	$(BOOST_CPPFLAGS)

AM_LDFLAGS := $(LDFLAGS_GCOV)

CLEANFILES = \
	*.zlog

check_PROGRAMS = zSocketUnitTest

zSocketUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zSocketTest.h \
    UnitTest.cpp \
    Defaults.cpp \
    Buffer.cpp \
    Address.cpp \
    LoopAddress.cpp \
    LoopSocket.cpp \
    UnixAddress.cpp \
    UnixSocket.cpp

zSocketUnitTest_LDADD = \
    ${top_builddir}/lib/zSocket/libzSocket.la \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zSem/libzSem.la
