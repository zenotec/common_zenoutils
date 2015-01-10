SUBDIRS = zData zEvent zException zGetOpt zLog zMessage zNode zSemaphore zSocket zThread zTimer

if COND_GCOV
GCOV_EXTRA_CPPFLAGS = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
GCOV_EXTRA_LDFLAGS = -lgcov
endif

AM_CPPFLAGS := $(GCOV_EXTRA_CPPFLAGS)
AM_LDFLAGS := $(GCOV_EXTRA_LDFLAGS)

lib_LTLIBRARIES = libzutils.la

# Where to install the headers on the system
libzutils_ladir = $(includedir)/zutils

# Headers to install
libzutils_la_HEADERS = \
    $(top_srcdir)/inc/zutils/zData.h \
    $(top_srcdir)/inc/zutils/zEvent.h \
    $(top_srcdir)/inc/zutils/zException.h \
    $(top_srcdir)/inc/zutils/zGetOpt.h \
    $(top_srcdir)/inc/zutils/zLog.h \
    $(top_srcdir)/inc/zutils/zMessage.h \
    $(top_srcdir)/inc/zutils/zNode.h \
    $(top_srcdir)/inc/zutils/zQueue.h \
    $(top_srcdir)/inc/zutils/zSemaphore.h \
    $(top_srcdir)/inc/zutils/zSocket.h \
    $(top_srcdir)/inc/zutils/zThread.h \
    $(top_srcdir)/inc/zutils/zTimer.h 

# Sources to include in the package
libzutils_la_SOURCES = \
    $(libzutils_la_HEADERS)

libzutils_la_LIBADD = \
    ${top_builddir}/src/zData/libzData.la \
    ${top_builddir}/src/zEvent/libzEvent.la \
    ${top_builddir}/src/zException/libzException.la \
    ${top_builddir}/src/zGetOpt/libzGetOpt.la \
    ${top_builddir}/src/zLog/libzLog.la \
    ${top_builddir}/src/zMessage/libzMessage.la \
    ${top_builddir}/src/zNode/libzNode.la \
    ${top_builddir}/src/zSemaphore/libzSemaphore.la \
    ${top_builddir}/src/zSocket/libzSocket.la \
    ${top_builddir}/src/zThread/libzThread.la \
    ${top_builddir}/src/zTimer/libzTimer.la
