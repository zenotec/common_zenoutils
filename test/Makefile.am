AM_CPPFLAGS = \
        -fprofile-arcs \
        -ftest-coverage \
        -I$(top_srcdir)/inc \
        -I$(top_srcdir)/test


AM_LDFLAGS = \
        -lrt \
        -lpthread \
        -fprofile-arcs


TESTS = check_zutils

check_PROGRAMS = check_zutils

check_zutils_SOURCES = \
        $(top_srcdir)/src/zLog.cpp \
        $(top_srcdir)/test/zLog_utest.cpp \
        $(top_srcdir)/src/zThread.cpp \
        $(top_srcdir)/test/zThread_utest.cpp \
        $(top_srcdir)/src/zSemaphore.cpp \
        $(top_srcdir)/test/zSemaphore_utest.cpp \
        $(top_srcdir)/src/zEvent.cpp \
        $(top_srcdir)/test/zEvent_utest.cpp \
        $(top_srcdir)/test/zQueue_utest.cpp \
        $(top_srcdir)/src/zTimer.cpp \
        $(top_srcdir)/test/zTimer_utest.cpp \
        $(top_srcdir)/src/zData.cpp \
        $(top_srcdir)/test/zData_utest.cpp \
        $(top_srcdir)/src/zNode.cpp \
        $(top_srcdir)/test/zNode_utest.cpp \
        $(top_srcdir)/test/UnitTest.cpp



