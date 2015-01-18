SUBDIRS = zLog zProgOpt zData zTimer zThread zEvent zQueue zSem zSocket zNode zMessage zGpio

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
    zGpio/valgrind.sh
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
    zGpio/zGpioUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
