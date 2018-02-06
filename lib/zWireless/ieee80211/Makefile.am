if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
	$(CPPFLAGS_GCOV) \
	$(com_incdir) \
	$(BOOST_CPPFLAGS) \
	-I${top_srcdir}/lib/zWireless/ieee80211 \
	-I$(srcdir)

AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libieee80211.la

libieee80211_la_SOURCES = \
	${top_srcdir}/lib/zWireless/ieee80211 \
	${top_srcdir}/inc/zutils/RadioTapField.h \
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
	${top_srcdir}/inc/zutils/RadioTap.h \
	RadioTap.cpp \
	${top_srcdir}/inc/zutils/Tag.h \
	${top_srcdir}/inc/zutils/ChannelsTag.h \
	${top_srcdir}/inc/zutils/CountryTag.h \
	${top_srcdir}/inc/zutils/DsssTag.h \
	${top_srcdir}/inc/zutils/ExtRatesTag.h \
	${top_srcdir}/inc/zutils/HtCapsTag.h \
	HtCapsTag.cpp \
	${top_srcdir}/inc/zutils/PowerCapsTag.h \
	${top_srcdir}/inc/zutils/RatesTag.h \
	${top_srcdir}/inc/zutils/SsidTag.h \
	${top_srcdir}/inc/zutils/TimTag.h \
	${top_srcdir}/inc/zutils/VendorSpecificTag.h \
	${top_srcdir}/inc/zutils/WmmWmeTag.h \
	${top_srcdir}/inc/zutils/Frame.h \
	Frame.cpp \
	${top_srcdir}/inc/zutils/ControlFrame.h \
	ControlFrame.cpp \
	${top_srcdir}/inc/zutils/ManagementFrame.h \
	ManagementFrame.cpp \
	${top_srcdir}/inc/zutils/Beacon.h \
	Beacon.cpp \
	${top_srcdir}/inc/zutils/Probe.h \
	Probe.cpp \
	${top_srcdir}/inc/zutils/Authentication.h \
	Authentication.cpp \
	${top_srcdir}/inc/zutils/Association.h \
	Association.cpp \
	${top_srcdir}/inc/zutils/DataFrame.h \
	DataFrame.cpp
	