SUBDIRS = \
	zConf \
	zData \
	zEvent \
	zException \
	zGpio \
	zLed \
	zLog \
	zSocket \
	zMessage \
	zNode \
	zProgOpt \
	zSem \
	zSwitch \
	zThread \
	zTimer
	

if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV)
AM_LDFLAGS := $(LDFLAGS_GCOV)

lib_LTLIBRARIES = libzutils.la

# Where to install the headers on the system
libzutils_ladir = $(includedir)/zutils

# Headers to install
libzutils_la_HEADERS = \
    $(top_srcdir)/inc/zutils/zConf.h \
    $(top_srcdir)/inc/zutils/zData.h \
    $(top_srcdir)/inc/zutils/zEvent.h \
    $(top_srcdir)/inc/zutils/zException.h \
    $(top_srcdir)/inc/zutils/zProgOpt.h \
    $(top_srcdir)/inc/zutils/zGpio.h \
    $(top_srcdir)/inc/zutils/zLed.h \
    $(top_srcdir)/inc/zutils/zLog.h \
    $(top_srcdir)/inc/zutils/zMessage.h \
    $(top_srcdir)/inc/zutils/zNode.h \
    $(top_srcdir)/inc/zutils/zQueue.h \
    $(top_srcdir)/inc/zutils/zSem.h \
    $(top_srcdir)/inc/zutils/zSocket.h \
    $(top_srcdir)/inc/zutils/zInet.h \
    $(top_srcdir)/inc/zutils/zEthSocket.h \
    $(top_srcdir)/inc/zutils/zSwitch.h \
    $(top_srcdir)/inc/zutils/zThread.h \
    $(top_srcdir)/inc/zutils/zTimer.h \
    $(top_srcdir)/inc/zutils/zUuid.h 

# Sources to include in the package
libzutils_la_SOURCES = \
    $(libzutils_la_HEADERS)

libzutils_la_LIBADD = \
    ${top_builddir}/src/zConf/libzConf.la \
    ${top_builddir}/src/zData/libzData.la \
    ${top_builddir}/src/zEvent/libzEvent.la \
    ${top_builddir}/src/zException/libzException.la \
    ${top_builddir}/src/zGpio/libzGpio.la \
    ${top_builddir}/src/zLed/libzLed.la \
    ${top_builddir}/src/zLog/libzLog.la \
    ${top_builddir}/src/zMessage/libzMessage.la \
    ${top_builddir}/src/zNode/libzNode.la \
    ${top_builddir}/src/zProgOpt/libzProgOpt.la \
    ${top_builddir}/src/zSem/libzSem.la \
    ${top_builddir}/src/zSocket/libzSocket.la \
    ${top_builddir}/src/zSwitch/libzSwitch.la \
    ${top_builddir}/src/zThread/libzThread.la \
    ${top_builddir}/src/zTimer/libzTimer.la
