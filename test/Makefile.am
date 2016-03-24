#SUBDIRS = \
#	zLog \
#	zProgOpt \
#	zData \
#	zConf \
#	zSwitch \
#	zThermo \
#	zCom \
#	zLed \
#	zLcd \
#	zSocket \
#	zMessage \
#	zCommand

SUBDIRS = \
	zThread \
	zTimer \
	zProgOpt \
	zLog \
	zSem \
	zQueue \
	zData \
	zEvent \
	zGpio

if COND_VALGRIND
#VALGRIND_TESTS = \
#    zProgOpt/valgrind.sh \
#    zLog/valgrind.sh \
#    zData/valgrind.sh \
#    zCommand/valgrind.sh \
#    zConf/valgrind.sh \
#    zSem/valgrind.sh \
#    zEvent/valgrind.sh \
#    zThread/valgrind.sh \
#    zTimer/valgrind.sh \
#    zQueue/valgrind.sh \
#    zSocket/valgrind.sh \
#    zMessage/valgrind.sh \
#    zNode/valgrind.sh \
#    zSwitch/valgrind.sh \
#    zThermo/valgrind.sh \
#    zCom/valgrind.sh \
#    zGpio/valgrind.sh \
#    zLed/valgrind.sh \
#    zLcd/valgrind.sh \
#    zSocket/valgrind.sh
endif

#GCOV_TESTS = \
#    zProgOpt/zProgOptUnitTest \
#    zLog/zLogUnitTest \
#    zData/zDataUnitTest \
#    zCommand/zCommandUnitTest \
#    zConf/zConfUnitTest \
#    zSwitch/zSwitchUnitTest \
#    zThermo/zThermoUnitTest \
#    zCom/zComUnitTest \
#    zLed/zLedUnitTest \
#    zLcd/zLcdUnitTest \
#    zSocket/zSocketUnitTest \
#    zMessage/zMessageUnitTest


GCOV_TESTS = \
    zThread/zThreadUnitTest \
    zTimer/zTimerUnitTest \
    zProgOpt/zProgOptUnitTest \
    zLog/zLogUnitTest \
    zSem/zSemUnitTest \
    zQueue/zQueueUnitTest \
    zData/zDataUnitTest \
    zEvent/zEventUnitTest \
    zGpio/zGpioUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
