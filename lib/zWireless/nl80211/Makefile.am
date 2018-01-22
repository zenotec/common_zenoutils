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
	BeaconHeadAttribute.h \
	BeaconIntervalAttribute.h \
	BeaconTailAttribute.h \
	ChannelTypeAttribute.h \
	ChannelWidthAttribute.h \
	DtimPeriodAttribute.h \
	FrequencyAttribute.h \
	IfIndexAttribute.h \
	IfNameAttribute.h \
	IfTypeAttribute.h \
	MacAttribute.h \
	PhyIndexAttribute.h \
	PhyNameAttribute.h \
	SsidAttribute.h \
	TxPowerLevelAttribute.h \
	TxPowerModeAttribute.h \
	ListPhysCommand.h \
	ListPhysCommand.cpp \
	GetPhyCommand.h \
	GetPhyCommand.cpp \
	SetPhyCommand.h \
	SetPhyCommand.cpp \
	ListInterfacesCommand.h \
	ListInterfacesCommand.cpp \
	GetInterfaceCommand.h \
	GetInterfaceCommand.cpp \
	SetInterfaceCommand.h \
	SetInterfaceCommand.cpp \
	NewInterfaceCommand.h \
	NewInterfaceCommand.cpp \
	DelInterfaceCommand.h \
	DelInterfaceCommand.cpp \
	ListStationsCommand.h \
	ListStationsCommand.cpp \
	GetStationCommand.h \
	GetStationCommand.cpp \
	SetStationCommand.h \
	SetStationCommand.cpp \
	NewStationCommand.h \
	NewStationCommand.cpp \
	DelStationCommand.h \
	DelStationCommand.cpp \
	SetChannelCommand.h \
	SetChannelCommand.cpp \
	SetBssCommand.h \
	SetBssCommand.cpp \
	NewBeaconCommand.h \
	NewBeaconCommand.cpp \
	SetBeaconCommand.h \
	SetBeaconCommand.cpp \
	StartApCommand.h \
	StartApCommand.cpp \
	StopApCommand.h \
	StopApCommand.cpp
	