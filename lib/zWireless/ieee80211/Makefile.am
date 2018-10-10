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

# Where to install the headers on the system
libieee80211_ladir = $(includedir)/zutils/ieee80211

# Headers to install
libieee80211_la_HEADERS = \
	${top_srcdir}/inc/zutils/ieee80211/ieee80211.h \
	${top_srcdir}/inc/zutils/ieee80211/RadioTap.h \
	${top_srcdir}/inc/zutils/ieee80211/RadioTapField.h \
	${top_srcdir}/inc/zutils/ieee80211/Tag.h \
	${top_srcdir}/inc/zutils/ieee80211/ChannelsTag.h \
	${top_srcdir}/inc/zutils/ieee80211/CountryTag.h \
	${top_srcdir}/inc/zutils/ieee80211/DsssTag.h \
	${top_srcdir}/inc/zutils/ieee80211/ExtRatesTag.h \
	${top_srcdir}/inc/zutils/ieee80211/HtCapsTag.h \
	${top_srcdir}/inc/zutils/ieee80211/VhtCapsTag.h \
	${top_srcdir}/inc/zutils/ieee80211/HtInfoTag.h \
	${top_srcdir}/inc/zutils/ieee80211/PowerCapsTag.h \
	${top_srcdir}/inc/zutils/ieee80211/RatesTag.h \
	${top_srcdir}/inc/zutils/ieee80211/SsidTag.h \
	${top_srcdir}/inc/zutils/ieee80211/TimTag.h \
	${top_srcdir}/inc/zutils/ieee80211/VendorSpecificTag.h \
    $(top_srcdir)/inc/zutils/ieee80211/ChannelSwitchTag.h \
	${top_srcdir}/inc/zutils/ieee80211/WmmWmeTag.h \
	${top_srcdir}/inc/zutils/ieee80211/Frame.h \
	${top_srcdir}/inc/zutils/ieee80211/ControlFrame.h \
	${top_srcdir}/inc/zutils/ieee80211/ManagementFrame.h \
	${top_srcdir}/inc/zutils/ieee80211/Beacon.h \
	${top_srcdir}/inc/zutils/ieee80211/Probe.h \
	${top_srcdir}/inc/zutils/ieee80211/Action.h \
	${top_srcdir}/inc/zutils/ieee80211/Authentication.h \
	${top_srcdir}/inc/zutils/ieee80211/Deauthentication.h \
	${top_srcdir}/inc/zutils/ieee80211/Association.h \
	${top_srcdir}/inc/zutils/ieee80211/Reassociation.h \
	${top_srcdir}/inc/zutils/ieee80211/Disassociation.h \
	${top_srcdir}/inc/zutils/ieee80211/ErpInfoTag.h \
	${top_srcdir}/inc/zutils/ieee80211/ExtCapsTag.h \
	${top_srcdir}/inc/zutils/ieee80211/SuppOpClassesTag.h \
	${top_srcdir}/inc/zutils/ieee80211/DataFrame.h \
	${top_srcdir}/inc/zutils/ieee80211/QosData.h \
	${top_srcdir}/inc/zutils/ieee80211/Socket.h

libieee80211_la_SOURCES = \
	RadioTap.cpp \
	RadioTapField.cpp \
	RadioTapFieldTsft.cpp \
	RadioTapFieldFlags.cpp \
	RadioTapFieldRate.cpp \
	RadioTapFieldMcs.cpp \
	RadioTapFieldTxPower.cpp \
	RadioTapFieldChannel.cpp \
	RadioTapFieldSignalLevel.cpp \
	RadioTapFieldSignalNoise.cpp \
	RadioTapFieldAntenna.cpp \
	RadioTapFieldRxFlags.cpp \
	RadioTapFieldTxFlags.cpp \
	RadioTapFieldDataRetries.cpp \
	HtCapsTag.cpp \
	VhtCapsTag.cpp \
	HtInfoTag.cpp \
	Frame.cpp \
	ControlFrame.cpp \
	ManagementFrame.cpp \
	Beacon.cpp \
	Probe.cpp \
	Action.cpp \
	Authentication.cpp \
	Deauthentication.cpp \
	Association.cpp \
	Reassociation.cpp \
	Disassociation.cpp \
	DataFrame.cpp \
	QosData.cpp \
	Socket.cpp
	
