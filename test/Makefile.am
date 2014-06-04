SUBDIRS = zSocket

AM_CPPFLAGS = \
        -I${top_srcdir}/inc \
        -I${top_srcdir}/test


AM_LDFLAGS = \
        -lrt \
        -lpthread


TESTS = check_zutils

check_PROGRAMS = check_zutils

check_zutils_SOURCES = \
                       zLog_utest.cpp \
                       zThread_utest.cpp \
                       zSemaphore_utest.cpp \
                       zEvent_utest.cpp \
                       zQueue_utest.cpp \
                       zTimer_utest.cpp \
                       zData_utest.cpp \
                       zNode_utest.cpp \
                       zSocket_utest.cpp \
                       UnitTest.h \
                       UnitTest.cpp


check_zutils_LDADD = \
    ${top_builddir}/test/zSocket/libzSocketTest.la \
    ${top_builddir}/src/libzutils.la

