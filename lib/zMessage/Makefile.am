if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzMessage.la

libzMessage_la_SOURCES = \
	MessageNotification.cpp \
    MessagePath.cpp \
    Message.cpp \
    HelloMessage.cpp \
    HelloObserver.cpp \
    ByeMessage.cpp \
	ByeObserver.cpp \
    AckMessage.cpp \
	AckObserver.cpp \
    CommandMessage.cpp \
    MessageFactory.cpp \
    MessageSocket.cpp
