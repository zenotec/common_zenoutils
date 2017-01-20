#SUBDIRS = \
#	zException \
#	zNode \
#

export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

SUBDIRS = \
	zCommand \
	zConfig \
	zData \
	zDisplay \
	zEvent \
	zGpio \
	zInterface \
	zLed \
	zLog \
	zMessage \
	zProgram \
	zSem \
	zSerial \
	zSignal \
	zSocket \
	zSwitch \
	zThermo \
	zThread	\
	zTimer

if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) -Werror -Wfatal-errors
AM_LDFLAGS := $(LDFLAGS_GCOV)

lib_LTLIBRARIES = libzutils.la

# Where to install the headers on the system
libzutils_ladir = $(includedir)/zutils

# Headers to install
libzutils_la_HEADERS = \
    $(top_srcdir)/inc/zutils/zAckMessage.h \
    $(top_srcdir)/inc/zutils/zByeMessage.h \
    $(top_srcdir)/inc/zutils/zCommand.h \
    $(top_srcdir)/inc/zutils/zCommandMessage.h \
    $(top_srcdir)/inc/zutils/zCompatibility.h \
    $(top_srcdir)/inc/zutils/zConfig.h \
    $(top_srcdir)/inc/zutils/zData.h \
    $(top_srcdir)/inc/zutils/zDisplay.h \
    $(top_srcdir)/inc/zutils/zEchoSerialPort.h \
    $(top_srcdir)/inc/zutils/zEthSocket.h \
    $(top_srcdir)/inc/zutils/zEvent.h \
    $(top_srcdir)/inc/zutils/zException.h \
    $(top_srcdir)/inc/zutils/zGpio.h \
    $(top_srcdir)/inc/zutils/zHelloMessage.h \
    $(top_srcdir)/inc/zutils/zInetSocket.h \
    $(top_srcdir)/inc/zutils/zInterface.h \
    $(top_srcdir)/inc/zutils/zLed.h \
    $(top_srcdir)/inc/zutils/zLog.h \
    $(top_srcdir)/inc/zutils/zLoopSocket.h \
    $(top_srcdir)/inc/zutils/zMessage.h \
    $(top_srcdir)/inc/zutils/zMessageSocket.h \
    $(top_srcdir)/inc/zutils/zNode.h \
    $(top_srcdir)/inc/zutils/zProgram.h \
    $(top_srcdir)/inc/zutils/zQueue.h \
    $(top_srcdir)/inc/zutils/zSem.h \
    $(top_srcdir)/inc/zutils/zSerial.h \
    $(top_srcdir)/inc/zutils/zSignal.h \
    $(top_srcdir)/inc/zutils/zSocket.h \
    $(top_srcdir)/inc/zutils/zSwitch.h \
    $(top_srcdir)/inc/zutils/zThermo.h \
    $(top_srcdir)/inc/zutils/zThread.h \
    $(top_srcdir)/inc/zutils/zTimer.h \
    $(top_srcdir)/inc/zutils/zTtySerialPort.h \
    $(top_srcdir)/inc/zutils/zUnixSocket.h \
    $(top_srcdir)/inc/zutils/zUtils.h \
    $(top_srcdir)/inc/zutils/zUuid.h \
    $(top_srcdir)/inc/zutils/zWirelessInterface.h

# Sources to include in the package
libzutils_la_SOURCES = \
    $(libzutils_la_HEADERS)

#libzutils_la_LIBADD = \
#    ${top_builddir}/lib/zException/libzException.la \
#    ${top_builddir}/lib/zNode/libzNode.la \
#

libzutils_la_LIBADD = \
    ${top_builddir}/lib/zLog/libzLog.la \
    ${top_builddir}/lib/zSem/libzSem.la \
    ${top_builddir}/lib/zSignal/libzSignal.la \
    ${top_builddir}/lib/zThread/libzThread.la \
    ${top_builddir}/lib/zTimer/libzTimer.la \
    ${top_builddir}/lib/zData/libzData.la \
    ${top_builddir}/lib/zEvent/libzEvent.la \
    ${top_builddir}/lib/zConfig/libzConfig.la \
    ${top_builddir}/lib/zGpio/libzGpio.la \
    ${top_builddir}/lib/zLed/libzLed.la \
    ${top_builddir}/lib/zSerial/libzSerial.la \
    ${top_builddir}/lib/zInterface/libzInterface.la \
    ${top_builddir}/lib/zSocket/libzSocket.la \
    ${top_builddir}/lib/zMessage/libzMessage.la \
    ${top_builddir}/lib/zCommand/libzCommand.la \
    ${top_builddir}/lib/zSwitch/libzSwitch.la \
    ${top_builddir}/lib/zThermo/libzThermo.la \
    ${top_builddir}/lib/zDisplay/libzDisplay.la \
    ${top_builddir}/lib/zProgram/libzProgram.la

