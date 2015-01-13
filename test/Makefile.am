SUBDIRS = zLog zOpt zData zTimer zThread zEvent zQueue zSem zMessage zSocket zNode

if COND_VALGRIND
VALGRIND_TESTS = \
    zLog/valgrind.sh \
    zData/valgrind.sh \
    zEvent/valgrind.sh \
    zMessage/valgrind.sh \
    zNode/valgrind.sh \
    zOpt/valgrind.sh \
    zQueue/valgrind.sh \
    zSem/valgrind.sh \
    zSocket/valgrind.sh \
    zThread/valgrind.sh \
    zTimer/valgrind.sh
endif

GCOV_TESTS = \
    zLog/zLogUnitTest \
    zData/zDataUnitTest \
    zEvent/zEventUnitTest \
    zMessage/zMessageUnitTest \
    zNode/zNodeUnitTest \
    zOpt/zOptUnitTest \
    zQueue/zQueueUnitTest \
    zSem/zSemUnitTest \
    zSocket/zSocketUnitTest \
    zThread/zThreadUnitTest \
    zTimer/zTimerUnitTest

TESTS = \
    $(GCOV_TESTS) $(VALGRIND_TESTS)
