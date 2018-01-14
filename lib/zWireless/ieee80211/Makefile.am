if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS) -I$(srcdir)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libieee80211.la

libieee80211_la_SOURCES = \
	RadioTapField.h \
	RadioTapField.cpp \
	RadioTapFieldTsft.cpp \
	RadioTapFieldFlags.cpp \
	RadioTapFieldRate.cpp \
	RadioTapFieldChannel.cpp \
	RadioTapFieldSignalLevel.cpp \
	RadioTapFieldSignalNoise.cpp \
	RadioTapFieldAntenna.cpp \
	RadioTapFieldRxFlags.cpp \
	RadioTapFieldTxFlags.cpp \
	RadioTap.h \
	RadioTap.cpp \
	Frame.h \
	Frame.cpp \
	CountryTag.h \
	ControlFrame.h \
	ControlFrame.cpp \
	ManagementFrame.h \
	ManagementFrame.cpp \
	Beacon.h \
	Beacon.cpp \
	Probe.h \
	Probe.cpp \
	HtCapsTag.h \
	HtCapsTag.cpp \
	Authentication.h \
	Authentication.cpp \
	Association.h \
	Association.cpp \
	DataFrame.h \
	DataFrame.cpp
	