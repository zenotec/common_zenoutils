if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS) -I$(srcdir)
AM_LDFLAGS := $(LDFLAGS_GCOV)

libnetlink_la_CPPFLAGS = \
  ${AM_CPPFLAGS} \
  ${NL3_CFLAGS} \
  ${NLGENL3_CFLAGS} \
  ${NLRTNL3_CFLAGS}

libnetlink_la_LDFLAGS = \
  ${AM_LIBS} \
  ${NL3_LIBS} \
  ${NLGENL3_LIBS} \
  ${NLRTNL3_LIBS}

noinst_LTLIBRARIES = libnetlink.la

libnetlink_la_SOURCES = \
	Attribute.h \
	Command.h \
	Command.cpp \
	Message.h \
	Message.cpp \
	GenericMessage.h \
	GenericMessage.cpp \
	RouteMessage.h \
	RouteMessage.cpp \
	Handler.h \
	Handler.cpp \
	Socket.h \
	Socket.cpp \
	GenericSocket.h \
	GenericSocket.cpp \
	RouteLink.h \
	RouteLink.cpp \
	RouteLinkEvent.h \
	RouteLinkEvent.cpp \
	RouteSocket.h \
	RouteSocket.cpp \
	ListLinksCommand.h \
	ListLinksCommand.cpp \
	GetLinkCommand.h \
	GetLinkCommand.cpp \
	SetLinkCommand.h \
	SetLinkCommand.cpp \
	NewLinkCommand.h \
	NewLinkCommand.cpp \
	DelLinkCommand.h \
	DelLinkCommand.cpp
	