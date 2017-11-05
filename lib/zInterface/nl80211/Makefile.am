if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS) -I$(srcdir) -I$(srcdir)/../netlink
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
	BeaconIntervalAttribute.h \
	DtimPeriodAttribute.h \
	FrequencyAttribute.h \
	IfIndexAttribute.h \
	IfNameAttribute.h \
	IfTypeAttribute.h \
	MacAttribute.h \
	ListPhysCommand.h \
	PhyIndexAttribute.h \
	PhyNameAttribute.h \
	SsidAttribute.h \
	ListPhysCommand.cpp \
	GetPhyCommand.h \
	GetPhyCommand.cpp \
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
	SetBssCommand.h \
	SetBssCommand.cpp \
	SetBeaconCommand.h \
	SetBeaconCommand.cpp \
	StartApCommand.h \
	StartApCommand.cpp \
	StopApCommand.h \
	StopApCommand.cpp
	