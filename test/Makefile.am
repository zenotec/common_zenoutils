export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

if COND_ZMESSAGE
ZMESSAGE_SUBDIRS = zMessage
ZMESSAGE_TESTS = zMessage/zMessageUnitTest
if COND_VALGRIND
ZMESSAGE_TESTS += zMessage/valgrind.sh
endif
endif

if COND_ZSOCKET
ZSOCKET_SUBDIRS = zSocket
ZSOCKET_TESTS = \
	zSocket/zSocketUnitTest \
	zSocket/ieee8023/Ieee8023UnitTest
if COND_VALGRIND
ZSOCKET_TESTS += zSocket/valgrind.sh
endif
endif

if COND_ZPROGRAM
ZPROGRAM_SUBDIRS = zProgram
ZPROGRAM_TESTS = zProgram/zProgramUnitTest
if COND_VALGRIND
ZPROGRAM_TESTS += zProgram/valgrind.sh
endif
endif

if COND_ZWIRELESS
ZWIRELESS_SUBDIRS = zWireless
ZWIRELESS_TESTS = \
	zWireless/zWirelessUnitTest \
	zWireless/nl80211/Nl80211UnitTest \
	zWireless/ieee80211/Ieee80211UnitTest
if COND_VALGRIND
ZWIRELESS_TESTS += \
	zWireless/valgrind.sh \
	zWireless/nl80211/valgrind.sh \
	zWireless/ieee80211/valgrind.sh
endif
endif

if COND_ZINTERFACE
ZINTERFACE_SUBDIRS = zInterface
ZINTERFACE_TESTS = zInterface/zInterfaceUnitTest
if COND_VALGRIND
ZINTERFACE_TESTS += zInterface/valgrind.sh
endif
endif

if COND_ZSERIAL
ZSERIAL_SUBDIRS = zSerial
ZSERIAL_TESTS = zSerial/zSerialUnitTest
if COND_VALGRIND
ZSERIAL_TESTS += zSerial/valgrind.sh
endif
endif

if COND_ZLED
ZLED_SUBDIRS = zLed
ZLED_TESTS = zLed/zLedUnitTest
if COND_VALGRIND
ZLED_TESTS += zLed/valgrind.sh
endif
endif

if COND_ZGPIO
ZGPIO_SUBDIRS = zGpio
ZGPIO_TESTS = zGpio/zGpioUnitTest
if COND_VALGRIND
ZGPIO_TESTS += zGpio/valgrind.sh
endif
endif

if COND_ZTIMER
ZTIMER_SUBDIRS = zTimer
ZTIMER_TESTS = zTimer/zTimerUnitTest
if COND_VALGRIND
ZTIMER_TESTS += zTimer/valgrind.sh
endif
endif

if COND_ZTHREAD
ZTHREAD_SUBDIRS = zThread
ZTHREAD_TESTS = zThread/zThreadUnitTest
if COND_VALGRIND
ZTHREAD_TESTS += zThread/valgrind.sh
endif
endif

if COND_ZSIGNAL
ZSIGNAL_SUBDIRS = zSignal
ZSIGNAL_TESTS = zSignal/zSignalUnitTest
if COND_VALGRIND
ZSIGNAL_TESTS += zSignal/valgrind.sh
endif
endif

if COND_ZCONFIG
ZCONFIG_SUBDIRS = zConfig
ZCONFIG_TESTS = zConfig/zConfigUnitTest
if COND_VALGRIND
ZCONFIG_TESTS += zConfig/valgrind.sh
endif
endif

if COND_ZEVENT
ZEVENT_SUBDIRS = zEvent
ZEVENT_TESTS = zEvent/zEventUnitTest
if COND_VALGRIND
ZEVENT_TESTS += zEvent/valgrind.sh
endif
endif

if COND_ZMATH
ZMATH_SUBDIRS = zMath
ZMATH_TESTS = zMath/zMathUnitTest
if COND_VALGRIND
ZMATH_TESTS += zMath/valgrind.sh
endif
endif

if COND_ZDATA
ZDATA_SUBDIRS = zData
ZDATA_TESTS = zData/zDataUnitTest
if COND_VALGRIND
ZDATA_TESTS += zData/valgrind.sh
endif
endif

if COND_ZQUEUE
ZQUEUE_SUBDIRS = zQueue
ZQUEUE_TESTS = zQueue/zQueueUnitTest
if COND_VALGRIND
ZQUEUE_TESTS += zQueue/valgrind.sh
endif
endif

if COND_ZUUID
ZUUID_SUBDIRS = zUuid
ZUUID_TESTS = zUuid/zUuidUnitTest
if COND_VALGRIND
ZUUID_TESTS += zUuid/valgrind.sh
endif
endif

if COND_ZLOG
ZLOG_SUBDIRS = zLog
ZLOG_TESTS = zLog/zLogUnitTest
if COND_VALGRIND
ZLOG_TESTS += zLog/valgrind.sh
endif
endif

if COND_ZSEM
ZSEM_SUBDIRS = zSem
ZSEM_TESTS = zSem/zSemUnitTest
if COND_VALGRIND
ZSEM_TESTS += zSem/valgrind.sh
endif
endif

SUBDIRS = \
	$(ZLOG_SUBDIRS) \
	$(ZSEM_SUBDIRS) \
	$(ZUUID_SUBDIRS) \
	$(ZQUEUE_SUBDIRS) \
	$(ZDATA_SUBDIRS) \
	$(ZMATH_SUBDIRS) \
	$(ZEVENT_SUBDIRS) \
	$(ZCONFIG_SUBDIRS) \
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

TESTS = \
	$(ZLOG_TESTS) \
	$(ZSEM_TESTS) \
	$(ZUUID_TESTS) \
	$(ZQUEUE_TESTS) \
	$(ZDATA_TESTS) \
	$(ZMATH_TESTS) \
	$(ZEVENT_TESTS) \
	$(ZCONFIG_TESTS) \
	$(ZTHREAD_TESTS) \
	$(ZTIMER_TESTS) \
	$(ZGPIO_TESTS) \
	$(ZLED_TESTS) \
	$(ZSERIAL_TESTS) \
	$(ZINTERFACE_TESTS) \
	$(ZWIRELESS_TESTS) \
	$(ZPROGRAM_TESTS) \
	$(ZSOCKET_TESTS) \
	$(ZMESSAGE_TESTS)

clean-local:
	rm -f *.zlog
