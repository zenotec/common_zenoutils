

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
                       ../src/zLog.cpp \
                       zLog_utest.cpp \
                       ../src/zThread.cpp \
                       zThread_utest.cpp \
                       ../src/zSemaphore.cpp \
                       zSemaphore_utest.cpp \
                       ../src/zEvent.cpp \
                       zEvent_utest.cpp \
                       zQueue_utest.cpp \
                       ../src/zTimer.cpp \
                       zTimer_utest.cpp \
                       ../src/zData.cpp \
                       zData_utest.cpp \
                       ../src/zNode.cpp \
                       zNode_utest.cpp \
                       ../src/zSocket/SocketAddress.cpp \
                       ../src/zSocket/SocketBuffer.cpp \
                       ../src/zSocket/Socket.cpp \
                       zSocket_utest.cpp \
                       zSocket/SocketAddr_utest.cpp \
                       zSocket/SocketBuffer_utest.cpp \
                       zSocket/Socket_utest.cpp \
                       UnitTest.h \
                       UnitTest.cpp



