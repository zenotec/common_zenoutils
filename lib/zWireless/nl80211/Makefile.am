if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := \
  $(CPPFLAGS_GCOV) \
  $(com_incdir) \
  $(BOOST_CPPFLAGS) \
  -I$(srcdir) \
  -I${top_srcdir}/lib/zInterface/netlink

AM_LDFLAGS := $(LDFLAGS_GCOV)

libnl80211_la_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS}

libnl80211_la_LDFLAGS = \
  ${AM_LIBS} \
  ${NL3_LIBS} \
  ${NLGENL3_LIBS}

noinst_LTLIBRARIES = libnl80211.la

# Where to install the headers on the system
libnl80211_ladir = $(includedir)/zutils/nl80211

# Headers to install
libnl80211_la_HEADERS = \
	${top_srcdir}/inc/zutils/nl80211/BeaconHeadAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/BeaconIntervalAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/BeaconTailAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/ProbeResponseAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/ChannelWidthAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/DtimPeriodAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/FrequencyAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/IfIndexAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/IfNameAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/IfTypeAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/MacAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/PhyIndexAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/PhyDevAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/PhyNameAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/PhyBandsAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/SsidAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/TxPowerLevelAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/TxPowerModeAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/ListPhysCommand.h \
	${top_srcdir}/inc/zutils/nl80211/GetPhyCommand.h \
	${top_srcdir}/inc/zutils/nl80211/SetPhyCommand.h \
	${top_srcdir}/inc/zutils/nl80211/ListInterfacesCommand.h \
	${top_srcdir}/inc/zutils/nl80211/GetInterfaceCommand.h \
	${top_srcdir}/inc/zutils/nl80211/SetInterfaceCommand.h \
	${top_srcdir}/inc/zutils/nl80211/NewInterfaceCommand.h \
	${top_srcdir}/inc/zutils/nl80211/DelInterfaceCommand.h \
	${top_srcdir}/inc/zutils/nl80211/ListStationsCommand.h \
	${top_srcdir}/inc/zutils/nl80211/GetStationCommand.h \
	${top_srcdir}/inc/zutils/nl80211/SetStationCommand.h \
	${top_srcdir}/inc/zutils/nl80211/NewStationCommand.h \
	${top_srcdir}/inc/zutils/nl80211/DelStationCommand.h \
	${top_srcdir}/inc/zutils/nl80211/SetBeaconCommand.h \
	${top_srcdir}/inc/zutils/nl80211/StartApCommand.h \
	${top_srcdir}/inc/zutils/nl80211/StopApCommand.h \
	${top_srcdir}/inc/zutils/nl80211/Socket.h \
	${top_srcdir}/inc/zutils/nl80211/CenterFrequency1Attribute.h \
	${top_srcdir}/inc/zutils/nl80211/CenterFrequency2Attribute.h 

libnl80211_la_SOURCES = \
	Socket.cpp \
	ListPhysCommand.cpp \
	GetPhyCommand.cpp \
	SetPhyCommand.cpp \
	ListInterfacesCommand.cpp \
	GetInterfaceCommand.cpp \
	SetInterfaceCommand.cpp \
	NewInterfaceCommand.cpp \
	DelInterfaceCommand.cpp \
	ListStationsCommand.cpp \
	GetStationCommand.cpp \
	SetStationCommand.cpp \
	NewStationCommand.cpp \
	DelStationCommand.cpp \
	SetBeaconCommand.cpp \
	StartApCommand.cpp \
	StopApCommand.cpp
	
