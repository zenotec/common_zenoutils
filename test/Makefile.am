AM_CPPFLAGS = -I$(top_srcdir)/inc
AM_LDFLAGS = -lrt -lpthread

TESTS = check_zutil

check_PROGRAMS = check_zutil
check_zutil_SOURCES = $(top_srcdir)/test/UnitTest.cpp \
                      $(top_srcdir)/src/zData.cpp \
                      $(top_srcdir)/src/zEvent.cpp \
                      $(top_srcdir)/src/zSemaphore.cpp \
                      $(top_srcdir)/src/zTimer.cpp \
                      $(top_srcdir)/src/zSelect.cpp \
                      $(top_srcdir)/src/zNode.cpp \
                      $(top_srcdir)/test/zData_utest.cpp \
                      $(top_srcdir)/test/zEvent_utest.cpp \
                      $(top_srcdir)/test/zNode_utest.cpp
check_zutil_CFLAGS = @CHECK_CFLAGS@

