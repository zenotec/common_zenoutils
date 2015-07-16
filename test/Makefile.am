SUBDIRS = \
	zLog \
	zProgOpt \
	zData \
	zTimer \
	zThread \
	zEvent \
	zQueue \
	zSem \
	zSocket \
	zSwitch \
	zNode \
	zMessage \
	zGpio \
	zLed

if COND_VALGRIND
VALGRIND_TESTS = \
    zProgOpt/valgrind.sh \
    zLog/valgrind.sh \
    zData/valgrind.sh \
    zSem/valgrind.sh \
    zEvent/valgrind.sh \
    zThread/valgrind.sh \
    zTimer/valgrind.sh \
    zQueue/valgrind.sh \
    zNode/valgrind.sh \
    zSocket/valgrind.sh \
    zMessage/valgrind.sh \
    zSwtich/valgrind.sh \
    zGpio/valgrind.sh \
    zLed/valgrind.sh
endif

GCOV_TESTS = \
    zProgOpt/zProgOptUnitTest \
    zLog/zLogUnitTest \
    zData/zDataUnitTest \
    zSem/zSemUnitTest \
    zEvent/zEventUnitTest \
    zThread/zThreadUnitTest \
    zTimer/zTimerUnitTest \
    zQueue/zQueueUnitTest \
    zNode/zNodeUnitTest \
    zSocket/zSocketUnitTest \
    zMessage/zMessageUnitTest \
    zSwitch/zSwitchUnitTest \
    zGpio/zGpioUnitTest \
    zLed/zLedUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
