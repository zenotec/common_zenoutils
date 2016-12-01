#SUBDIRS = \
#	zException \
#

SUBDIRS = \
	zLog \
	zProgOpt \
	zSem \
	zThread	\
	zData \
	zEvent \
	zSignal \
	zTimer	\
	zConfig \
	zGpio \
	zLed \
	zSerial \
	zSocket \
	zMessage \
	zCommand \
	zSwitch \
	zLcd \
	zThermo

if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV)
AM_LDFLAGS := $(LDFLAGS_GCOV)

lib_LTLIBRARIES = libzutils.la

# Where to install the headers on the system
libzutils_ladir = $(includedir)/zutils

# Headers to install
libzutils_la_HEADERS = \
    $(top_srcdir)/inc/zutils/zData.h \
    $(top_srcdir)/inc/zutils/zCommand.h \
    $(top_srcdir)/inc/zutils/zConfig.h \
    $(top_srcdir)/inc/zutils/zEvent.h \
    $(top_srcdir)/inc/zutils/zException.h \
    $(top_srcdir)/inc/zutils/zProgOpt.h \
    $(top_srcdir)/inc/zutils/zGpio.h \
    $(top_srcdir)/inc/zutils/zLed.h \
    $(top_srcdir)/inc/zutils/zLcd.h \
    $(top_srcdir)/inc/zutils/zLog.h \
    $(top_srcdir)/inc/zutils/zMessage.h \
    $(top_srcdir)/inc/zutils/zNode.h \
    $(top_srcdir)/inc/zutils/zQueue.h \
    $(top_srcdir)/inc/zutils/zSem.h \
    $(top_srcdir)/inc/zutils/zSocket.h \
    $(top_srcdir)/inc/zutils/zLoopSocket.h \
    $(top_srcdir)/inc/zutils/zInetSocket.h \
    $(top_srcdir)/inc/zutils/zUnixSocket.h \
    $(top_srcdir)/inc/zutils/zEthSocket.h \
    $(top_srcdir)/inc/zutils/zSerial.h \
    $(top_srcdir)/inc/zutils/zSignal.h \
    $(top_srcdir)/inc/zutils/zSwitch.h \
    $(top_srcdir)/inc/zutils/zThermo.h \
    $(top_srcdir)/inc/zutils/zThread.h \
    $(top_srcdir)/inc/zutils/zTimer.h \
    $(top_srcdir)/inc/zutils/zEchoSerialPort.h \
    $(top_srcdir)/inc/zutils/zTtySerialPort.h \
    $(top_srcdir)/inc/zutils/zUuid.h

# Sources to include in the package
libzutils_la_SOURCES = \
    $(libzutils_la_HEADERS)

#libzutils_la_LIBADD = \
#    ${top_builddir}/src/zException/libzException.la \
#

libzutils_la_LIBADD = \
    ${top_builddir}/src/zLog/libzLog.la \
    ${top_builddir}/src/zProgOpt/libzProgOpt.la \
    ${top_builddir}/src/zSem/libzSem.la \
    ${top_builddir}/src/zThread/libzThread.la \
    ${top_builddir}/src/zData/libzData.la \
    ${top_builddir}/src/zEvent/libzEvent.la \
    ${top_builddir}/src/zSignal/libzSignal.la \
    ${top_builddir}/src/zTimer/libzTimer.la \
    ${top_builddir}/src/zConfig/libzConfig.la \
    ${top_builddir}/src/zGpio/libzGpio.la \
    ${top_builddir}/src/zLed/libzLed.la \
    ${top_builddir}/src/zSerial/libzSerial.la \
    ${top_builddir}/src/zSocket/libzSocket.la \
    ${top_builddir}/src/zMessage/libzMessage.la \
    ${top_builddir}/src/zCommand/libzCommand.la \
    ${top_builddir}/src/zSwitch/libzSwitch.la \
    ${top_builddir}/src/zThermo/libzThermo.la \
    ${top_builddir}/src/zLcd/libzLcd.la
    
    
