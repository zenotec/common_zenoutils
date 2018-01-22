if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS) -I$(srcdir)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libieee80211.la

libieee80211_la_SOURCES = \
	ieee80211.h \
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
	Tag.h \
	ChannelsTag.h \
	CountryTag.h \
	DsssTag.h \
	ExtRatesTag.h \
	HtCapsTag.h \
	HtCapsTag.cpp \
	PowerCapsTag.h \
	RatesTag.h \
	SsidTag.h \
	TimTag.h \
	VendorSpecificTag.h \
	WmmWmeTag.h \
	Frame.h \
	Frame.cpp \
	ControlFrame.h \
	ControlFrame.cpp \
	ManagementFrame.h \
	ManagementFrame.cpp \
	Beacon.h \
	Beacon.cpp \
	Probe.h \
	Probe.cpp \
	Authentication.h \
	Authentication.cpp \
	Association.h \
	Association.cpp \
	DataFrame.h \
	DataFrame.cpp
	