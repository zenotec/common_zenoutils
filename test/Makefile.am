SUBDIRS = \
	zLog \
	zProgOpt \
	zData \
	zConf \
	zTimer \
	zThread \
	zEvent \
	zQueue \
	zSem \
	zSocket \
	zSwitch \
	zMessage \
	zNode \
	zGpio \
	zLed

if COND_VALGRIND
VALGRIND_TESTS = \
    zProgOpt/valgrind.sh \
    zLog/valgrind.sh \
    zData/valgrind.sh \
    zConf/valgrind.sh \
    zSem/valgrind.sh \
    zEvent/valgrind.sh \
    zThread/valgrind.sh \
    zTimer/valgrind.sh \
    zQueue/valgrind.sh \
    zSocket/valgrind.sh \
    zMessage/valgrind.sh \
    zNode/valgrind.sh \
    zSwitch/valgrind.sh \
    zGpio/valgrind.sh \
    zLed/valgrind.sh
endif

GCOV_TESTS = \
    zProgOpt/zProgOptUnitTest \
    zLog/zLogUnitTest \
    zData/zDataUnitTest \
    zConf/zConfUnitTest \
    zSem/zSemUnitTest \
    zEvent/zEventUnitTest \
    zThread/zThreadUnitTest \
    zTimer/zTimerUnitTest \
    zQueue/zQueueUnitTest \
    zSocket/zSocketUnitTest \
    zMessage/zMessageUnitTest \
    zNode/zNodeUnitTest \
    zSwitch/zSwitchUnitTest \
    zGpio/zGpioUnitTest \
    zLed/zLedUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
