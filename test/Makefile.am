export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

SUBDIRS = \
	zLog \
	zMath \
	zThread \
	zProgram \
	zSem \
	zQueue \
	zData \
	zEvent \
	zTimer \
	zConfig \
	zGpio \
	zLed \
	zSerial \
	zInterface \
	zSocket \
	zMessage \
	zCommand \
	zSwitch \
	zThermo \
	zDisplay


if COND_VALGRIND
VALGRIND_TESTS = \
    zProgram/valgrind.sh \
    zLog/valgrind.sh \
    zData/valgrind.sh \
    zDisplay/valgrind.sh \
    zCommand/valgrind.sh \
    zConf/valgrind.sh \
    zSem/valgrind.sh \
    zEvent/valgrind.sh \
    zThread/valgrind.sh \
    zTimer/valgrind.sh \
    zQueue/valgrind.sh \
    zInterface/valgrind.sh \
    zSocket/valgrind.sh \
    zMessage/valgrind.sh \
    zNode/valgrind.sh \
    zSwitch/valgrind.sh \
    zThermo/valgrind.sh \
    zCom/valgrind.sh \
    zGpio/valgrind.sh \
    zLed/valgrind.sh
endif

GCOV_TESTS = \
    zLog/zLogUnitTest \
    zMath/zMathUnitTest \
    zThread/zThreadUnitTest \
    zProgram/zProgramUnitTest \
    zSem/zSemUnitTest \
    zQueue/zQueueUnitTest \
    zData/zDataUnitTest \
    zEvent/zEventUnitTest \
    zTimer/zTimerUnitTest \
    zConfig/zConfigUnitTest \
    zGpio/zGpioUnitTest \
    zLed/zLedUnitTest \
    zSerial/zSerialUnitTest \
    zInterface/zInterfaceUnitTest \
    zSocket/zSocketUnitTest \
    zMessage/zMessageUnitTest \
    zCommand/zCommandUnitTest \
    zSwitch/zSwitchUnitTest \
    zThermo/zThermoUnitTest \
    zDisplay/zDisplayUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
