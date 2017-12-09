if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS) -I$(srcdir)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libieee80211.la

libieee80211_la_SOURCES = \
	$(top_srcdir)/inc/ieee80211/RadioTapField.h \
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
	$(top_srcdir)/inc/ieee80211/RadioTap.h \
	RadioTap.cpp \
	$(top_srcdir)/inc/ieee80211/Frame.h \
	Frame.cpp \
	$(top_srcdir)/inc/ieee80211/ControlFrame.h \
	ControlFrame.cpp \
	$(top_srcdir)/inc/ieee80211/ManagementFrame.h \
	ManagementFrame.cpp \
	$(top_srcdir)/inc/ieee80211/Beacon.h \
	Beacon.cpp \
	$(top_srcdir)/inc/ieee80211/Probe.h \
	Probe.cpp \
	$(top_srcdir)/inc/ieee80211/HtCapsTag.h \
	HtCapsTag.cpp \
	$(top_srcdir)/inc/ieee80211/Association.h \
	Association.cpp \
	$(top_srcdir)/inc/ieee80211/DataFrame.h \
	DataFrame.cpp
	