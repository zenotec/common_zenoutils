if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
	$(CPPFLAGS_GCOV) \
	$(com_incdir) \
	-I${top_srcdir}/test/inc \
	-I${top_srcdir}/lib/zSocket/ieee8023 \
	-I${top_srcdir}/lib/zWireless/ieee80211 \
    $(OPENSSL_INCLUDES) \
	$(BOOST_CPPFLAGS) \
	$(OPENSSL_LIBS)
	

AM_LDFLAGS := \
    $(LDFLAGS_GCOV) \
    $(OPENSSL_LDFLAGS)

AM_LIBFLAGS := \
    $(OPENSSL_LIBS)

CLEANFILES = \
	*.zlog

check_PROGRAMS = zSecurityUnitTest

zSecurityUnitTest_SOURCES = \
    $(top_srcdir)/test/inc/UnitTest.h \
    valgrind.sh \
    zSecurityTest.h \
    zSecurityTest.cpp \
    UnitTest.cpp 


zSecurityUnitTest_LDADD = \
    ${top_builddir}/lib/zSecurity/libzSecurity.la \
    ${top_builddir}/lib/zSocket/libzSocket.la \
    ${top_builddir}/lib/zSocket/ieee8023/libieee8023.la \
    ${top_builddir}/lib/zWireless/ieee80211/libieee80211.la \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zSem/libzSem.la