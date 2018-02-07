if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
	$(CPPFLAGS_GCOV) \
	$(com_incdir) \
	$(BOOST_CPPFLAGS) \
	-I${top_srcdir}/lib/zWireless/ieee8023 \
	-I$(srcdir)

AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libieee8023.la

libieee8023_la_SOURCES = \
	ieee8023.h \
	${top_srcdir}/inc/zutils/ieee8023/Frame.h \
	Frame.cpp
