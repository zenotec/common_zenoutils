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

libnl80211_la_SOURCES = \
	${top_srcdir}/inc/zutils/nl80211/BeaconHeadAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/BeaconIntervalAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/BeaconTailAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/ChannelTypeAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/ChannelWidthAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/DtimPeriodAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/FrequencyAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/IfIndexAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/IfNameAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/IfTypeAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/MacAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/PhyIndexAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/PhyNameAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/SsidAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/TxPowerLevelAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/TxPowerModeAttribute.h \
	${top_srcdir}/inc/zutils/nl80211/ListPhysCommand.h \
	ListPhysCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/GetPhyCommand.h \
	GetPhyCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/SetPhyCommand.h \
	SetPhyCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/ListInterfacesCommand.h \
	ListInterfacesCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/GetInterfaceCommand.h \
	GetInterfaceCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/SetInterfaceCommand.h \
	SetInterfaceCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/NewInterfaceCommand.h \
	NewInterfaceCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/DelInterfaceCommand.h \
	DelInterfaceCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/ListStationsCommand.h \
	ListStationsCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/GetStationCommand.h \
	GetStationCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/SetStationCommand.h \
	SetStationCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/NewStationCommand.h \
	NewStationCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/DelStationCommand.h \
	DelStationCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/SetChannelCommand.h \
	SetChannelCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/SetBssCommand.h \
	SetBssCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/NewBeaconCommand.h \
	NewBeaconCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/SetBeaconCommand.h \
	SetBeaconCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/StartApCommand.h \
	StartApCommand.cpp \
	${top_srcdir}/inc/zutils/nl80211/StopApCommand.h \
	StopApCommand.cpp
	