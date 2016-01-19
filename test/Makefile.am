SUBDIRS = \
	zLog \
	zProgOpt \
	zData \
	zConf \
	zSem \
	zEvent \
	zQueue \
	zThread \
	zTimer \
	zSwitch \
	zThermo \
	zCom \
	zGpio \
	zLed \
	zLcd \
	zSocket \
	zMessage \
	zCommand

if COND_VALGRIND
VALGRIND_TESTS = \
    zProgOpt/valgrind.sh \
    zLog/valgrind.sh \
    zData/valgrind.sh \
    zCommand/valgrind.sh \
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
    zThermo/valgrind.sh \
    zCom/valgrind.sh \
    zGpio/valgrind.sh \
    zLed/valgrind.sh \
    zLcd/valgrind.sh \
    zSocket/valgrind.sh
endif

GCOV_TESTS = \
    zProgOpt/zProgOptUnitTest \
    zLog/zLogUnitTest \
    zData/zDataUnitTest \
    zCommand/zCommandUnitTest \
    zConf/zConfUnitTest \
    zSem/zSemUnitTest \
    zEvent/zEventUnitTest \
    zThread/zThreadUnitTest \
    zTimer/zTimerUnitTest \
    zQueue/zQueueUnitTest \
    zSwitch/zSwitchUnitTest \
    zThermo/zThermoUnitTest \
    zCom/zComUnitTest \
    zGpio/zGpioUnitTest \
    zLed/zLedUnitTest \
    zLcd/zLcdUnitTest \
    zSocket/zSocketUnitTest \
    zMessage/zMessageUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
