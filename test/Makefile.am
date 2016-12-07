
SUBDIRS = \
	zThread \
	zProgOpt \
	zLog \
	zSem \
	zQueue \
	zData \
	zEvent \
	zTimer \
	zConfig \
	zGpio \
	zLed \
	zSerial \
	zSocket \
	zMessage \
	zCommand \
	zSwitch \
	zThermo \
	zDisplay


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
    zDisplay/valgrind.sh \
    zSocket/valgrind.sh
endif

GCOV_TESTS = \
    zThread/zThreadUnitTest \
    zProgOpt/zProgOptUnitTest \
    zLog/zLogUnitTest \
    zSem/zSemUnitTest \
    zQueue/zQueueUnitTest \
    zData/zDataUnitTest \
    zEvent/zEventUnitTest \
    zTimer/zTimerUnitTest \
    zConfig/zConfigUnitTest \
    zGpio/zGpioUnitTest \
    zLed/zLedUnitTest \
    zSerial/zSerialUnitTest \
    zSocket/zSocketUnitTest \
    zMessage/zMessageUnitTest \
    zCommand/zCommandUnitTest \
    zSwitch/zSwitchUnitTest \
    zThermo/zThermoUnitTest \
    zDisplay/zDisplayUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
