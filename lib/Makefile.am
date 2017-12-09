export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

if COND_ZMESSAGE
ZMESSAGE_SUBDIRS = zMessage
ZMESSAGE_SOURCE = \
	$(top_srcdir)/inc/zutils/zMessage.h \
	$(top_srcdir)/inc/zutils/zAckMessage.h \
	$(top_srcdir)/inc/zutils/zByeMessage.h \
	$(top_srcdir)/inc/zutils/zHelloMessage.h
ZMESSAGE_CPPFLAGS =
ZMESSAGE_LDFLAGS =
ZMESSAGE_LIBS = \
	${top_builddir}/lib/zMessage/libzMessage.la
endif

if COND_ZSOCKET
ZSOCKET_SUBDIRS = zSocket
ZSOCKET_SOURCE = \
	$(top_srcdir)/inc/zutils/zSocket.h \
	$(top_srcdir)/inc/zutils/zEthSocket.h \
	$(top_srcdir)/inc/zutils/zInetSocket.h \
	$(top_srcdir)/inc/zutils/zLoopSocket.h \
	$(top_srcdir)/inc/zutils/zUnixSocket.h
ZSOCKET_CPPFLAGS =
ZSOCKET_LDFLAGS =
ZSOCKET_LIBS = \
	${top_builddir}/lib/zSocket/libzSocket.la
endif

if COND_ZPROGRAM
ZPROGRAM_SUBDIRS = zProgram
ZPROGRAM_SOURCE = $(top_srcdir)/inc/zutils/zProgram.h
ZPROGRAM_CPPFLAGS =
ZPROGRAM_LDFLAGS =
ZPROGRAM_LIBS = \
	${top_builddir}/lib/zProgram/libzProgram.la
endif

if COND_ZWIRELESS
ZWIRELESS_SUBDIRS = zWireless
ZWIRELESS_SOURCE = \
	$(top_srcdir)/inc/zutils/zWirelessInterface.h \
	$(top_srcdir)/inc/zutils/zMonitorInterface.h \
	$(top_srcdir)/inc/zutils/zAccessPointInterface.h
ZWIRELESS_CPPFLAGS =
ZWIRELESS_LDFLAGS =
ZWIRELESS_LIBS = \
	${top_builddir}/lib/zWireless/libzWireless.la
endif

if COND_ZINTERFACE
ZINTERFACE_SUBDIRS = zInterface
ZINTERFACE_SOURCE = \
	$(top_srcdir)/inc/zutils/zInterface.h \
	$(top_srcdir)/inc/zutils/zLoopInterface.h
ZINTERFACE_CPPFLAGS =
ZINTERFACE_LDFLAGS =
ZINTERFACE_LIBS = \
	${top_builddir}/lib/zInterface/libzInterface.la
endif

if COND_ZSERIAL
ZSERIAL_SUBDIRS = zSerial
ZSERIAL_SOURCE = \
	$(top_srcdir)/inc/zutils/zSerial.h \
	$(top_srcdir)/inc/zutils/zEchoSerialPort.h \
	$(top_srcdir)/inc/zutils/zTtySerialPort.h
ZSERIAL_CPPFLAGS =
ZSERIAL_LDFLAGS =
ZSERIAL_LIBS = \
	${top_builddir}/lib/zSerial/libzSerial.la
endif

if COND_ZLED
ZLED_SUBDIRS = zLed
ZLED_SOURCE = \
	$(top_srcdir)/inc/zutils/zLed.h
ZLED_CPPFLAGS =
ZLED_LDFLAGS =
ZLED_LIBS = \
	${top_builddir}/lib/zLed/libzLed.la
endif

if COND_ZGPIO
ZGPIO_SUBDIRS = zGpio
ZGPIO_SOURCE = \
	$(top_srcdir)/inc/zutils/zGpio.h
ZGPIO_CPPFLAGS =
ZGPIO_LDFLAGS =
ZGPIO_LIBS = \
	${top_builddir}/lib/zGpio/libzGpio.la
endif

if COND_ZTIMER
ZTIMER_SUBDIRS = zTimer
ZTIMER_SOURCE = \
	$(top_srcdir)/inc/zutils/zTimer.h
ZTIMER_CPPFLAGS =
ZTIMER_LDFLAGS =
ZTIMER_LIBS = \
	${top_builddir}/lib/zTimer/libzTimer.la
endif

if COND_ZTHREAD
ZTHREAD_SUBDIRS = zThread
ZTHREAD_SOURCE = \
	$(top_srcdir)/inc/zutils/zThread.h
ZTHREAD_CPPFLAGS =
ZTHREAD_LDFLAGS =
ZTHREAD_LIBS = \
	${top_builddir}/lib/zThread/libzThread.la
endif

if COND_ZSIGNAL
ZSIGNAL_SUBDIRS = zSignal
ZSIGNAL_SOURCE = \
	$(top_srcdir)/inc/zutils/zSignal.h
ZSIGNAL_CPPFLAGS =
ZSIGNAL_LDFLAGS =
ZSIGNAL_LIBS = \
	${top_builddir}/lib/zSignal/libzSignal.la
endif

if COND_ZEVENT
ZEVENT_SUBDIRS = zEvent
ZEVENT_SOURCE = \
	$(top_srcdir)/inc/zutils/zEvent.h
ZEVENT_CPPFLAGS =
ZEVENT_LDFLAGS =
ZEVENT_LIBS = \
	${top_builddir}/lib/zEvent/libzEvent.la
endif

if COND_ZMATH
ZMATH_SUBDIRS = zMath
ZMATH_SOURCE = \
	$(top_srcdir)/inc/zutils/zMath.h
ZMATH_CPPFLAGS =
ZMATH_LDFLAGS =
ZMATH_LIBS = \
	${top_builddir}/lib/zMath/libzMath.la
endif

if COND_ZCONFIG
ZCONFIG_SUBDIRS = zConfig
ZCONFIG_SOURCE = \
	$(top_srcdir)/inc/zutils/zConfig.h
ZCONFIG_CPPFLAGS =
ZCONFIG_LDFLAGS =
ZCONFIG_LIBS = \
	${top_builddir}/lib/zConfig/libzConfig.la
endif

if COND_ZDATA
ZDATA_SUBDIRS = zData
ZDATA_SOURCE = \
	$(top_srcdir)/inc/zutils/zData.h
ZDATA_CPPFLAGS =
ZDATA_LDFLAGS =
ZDATA_LIBS = \
	${top_builddir}/lib/zData/libzData.la
endif

if COND_ZQUEUE
ZQUEUE_SOURCE = \
	$(top_srcdir)/inc/zutils/zQueue.h
ZQUEUE_CPPFLAGS =
ZQUEUE_LDFLAGS =
endif

if COND_ZSEM
ZSEM_SUBDIRS = zSem
ZSEM_SOURCE = \
	$(top_srcdir)/inc/zutils/zSem.h
ZSEM_CPPFLAGS =
ZSEM_LDFLAGS =
ZSEM_LIBS = \
	${top_builddir}/lib/zSem/libzSem.la
endif

if COND_ZLOG
ZLOG_SUBDIRS = zLog
ZLOG_SOURCE = \
	$(top_srcdir)/inc/zutils/zLog.h
ZLOG_CPPFLAGS =
ZLOG_LDFLAGS =
ZLOG_LIBS = \
	${top_builddir}/lib/zLog/libzLog.la
endif

if COND_GCOV
GCOV_CPPFLAGS = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
GCOV_LDFLAGS = -lgcov
endif

SUBDIRS = \
	$(ZLOG_SUBDIRS) \
	$(ZSEM_SUBDIRS) \
	$(ZQUEUE_SUBDIRS) \
	$(ZDATA_SUBDIRS) \
	$(ZMATH_SUBDIRS) \
	$(ZEVENT_SUBDIRS) \
	$(ZCONFIG_SUBDIRS) \
	$(ZSIGNAL_SUBDIRS) \
	$(ZTHREAD_SUBDIRS) \
	$(ZTIMER_SUBDIRS) \
	$(ZGPIO_SUBDIRS) \
	$(ZLED_SUBDIRS) \
	$(ZSERIAL_SUBDIRS) \
	$(ZINTERFACE_SUBDIRS) \
	$(ZWIRELESS_SUBDIRS) \
	$(ZPROGRAM_SUBDIRS) \
	$(ZSOCKET_SUBDIRS) \
	$(ZMESSAGE_SUBDIRS)

AM_CPPFLAGS := \
	$(GCOV_CPPFLAGS) \
	$(ZLOG_CPPFLAGS) \
	$(ZSEM_CPPFLAGS) \
	$(ZQUEUE_CPPFLAGS) \
	$(ZDATA_CPPFLAGS) \
	$(ZMATH_CPPFLAGS) \
	$(ZEVENT_CPPFLAGS) \
	$(ZCONFIG_CPPFLAGS) \
	$(ZSIGNAL_CPPFLAGS) \
	$(ZTHREAD_CPPFLAGS) \
	$(ZTIMER_CPPFLAGS) \
	$(ZGPIO_CPPFLAGS) \
	$(ZLED_CPPFLAGS) \
	$(ZSERIAL_CPPFLAGS) \
	$(ZINTERFACE_CPPFLAGS) \
	$(ZWIRELESS_CPPFLAGS) \
	$(ZPROGRAM_CPPFLAGS) \
	$(ZSOCKET_CPPFLAGS) \
	$(ZMESSAGE_CPPFLAGS) \
	-Werror -Wfatal-errors

AM_LDFLAGS := \
	$(GCOV_LDFLAGS) \
	$(ZLOG_LDFLAGS) \
	$(ZSEM_LDFLAGS) \
	$(ZQUEUE_LDFLAGS) \
	$(ZDATA_LDFLAGS) \
	$(ZMATH_LDFLAGS) \
	$(ZEVENT_LDFLAGS) \
	$(ZCONFIG_LDFLAGS) \
	$(ZSIGNAL_LDFLAGS) \
	$(ZTHREAD_LDFLAGS) \
	$(ZTIMER_LDFLAGS) \
	$(ZGPIO_LDFLAGS) \
	$(ZLED_LDFLAGS) \
	$(ZSERIAL_LDFLAGS) \
	$(ZINTERFACE_LDFLAGS) \
	$(ZWIRELESS_LDFLAGS) \
	$(ZPROGRAM_LDFLAGS) \
	$(ZSOCKET_LDFLAGS) \
	$(ZMESSAGE_LDFLAGS)

lib_LTLIBRARIES = libzutils.la

# Where to install the headers on the system
libzutils_ladir = $(includedir)/zutils

# Headers to install
libzutils_la_HEADERS = \
	$(top_srcdir)/inc/zutils/zCompatibility.h \
	$(top_srcdir)/inc/zutils/zUtils.h \
	$(top_srcdir)/inc/zutils/zUuid.h \
	$(ZLOG_SOURCE) \
	$(ZSEM_SOURCE) \
	$(ZQUEUE_SOURCE) \
	$(ZDATA_SOURCE) \
	$(ZMATH_SOURCE) \
	$(ZEVENT_SOURCE) \
	$(ZCONFIG_SOURCE) \
	$(ZSIGNAL_SOURCE) \
	$(ZTHREAD_SOURCE) \
	$(ZTIMER_SOURCE) \
	$(ZGPIO_SOURCE) \
	$(ZLED_SOURCE) \
	$(ZSERIAL_SOURCE) \
	$(ZINTERFACE_SOURCE) \
	$(ZWIRELESS_SOURCE) \
	$(ZPROGRAM_SOURCE) \
	$(ZSOCKET_SOURCE) \
	$(ZMESSAGE_SOURCE)

# Sources to include in the package
libzutils_la_SOURCES = \
    $(libzutils_la_HEADERS)

libzutils_la_LIBADD = \
	$(ZLOG_LIBS) \
	$(ZSEM_LIBS) \
	$(ZQUEUE_LIBS) \
	$(ZDATA_LIBS) \
	$(ZMATH_LIBS) \
	$(ZEVENT_LIBS) \
	$(ZCONFIG_LIBS) \
	$(ZSIGNAL_LIBS) \
	$(ZTHREAD_LIBS) \
 	$(ZTIMER_LIBS) \
	$(ZGPIO_LIBS) \
	$(ZLED_LIBS) \
	$(ZSERIAL_LIBS) \
	$(ZINTERFACE_LIBS) \
	$(ZWIRELESS_LIBS) \
	$(ZPROGRAM_LIBS) \
	$(ZSOCKET_LIBS) \
	$(ZMESSAGE_LIBS)
