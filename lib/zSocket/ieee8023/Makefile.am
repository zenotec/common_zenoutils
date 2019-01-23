if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
	$(CPPFLAGS_GCOV) \
	$(com_incdir) \
	$(BOOST_CPPFLAGS) \
	-I${top_srcdir}/lib/zSocket/ieee8023 \
	-I$(srcdir)

AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libieee8023.la

# Where to install the headers on the system
libieee8023_ladir = $(includedir)/zutils/ieee8023

# Headers to install
libieee8023_la_HEADERS = \
	${top_srcdir}/inc/zutils/ieee8023/Frame.h \
	${top_srcdir}/inc/zutils/ieee8023/EtherFrame.h \
	${top_srcdir}/inc/zutils/ieee8023/Ether2Frame.h \
	${top_srcdir}/inc/zutils/ieee8023/VlanFrame.h \
	${top_srcdir}/inc/zutils/ieee8023/LlcFrame.h \
	${top_srcdir}/inc/zutils/ieee8023/EapolFrame.h \
	${top_srcdir}/inc/zutils/ieee8023/Socket.h

libieee8023_la_SOURCES = \
	ieee8023.h \
	Frame.cpp \
	EtherFrame.cpp \
	LlcFrame.cpp \
	Ether2Frame.cpp \
	VlanFrame.cpp \
	EapolFrame.cpp \
	Socket.cpp
