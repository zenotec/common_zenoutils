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

# Where to install the headers on the system
libnetlink_ladir = $(includedir)/zutils/netlink

# Headers to install
libnetlink_la_HEADERS = \
	${top_srcdir}/inc/zutils/netlink/Attribute.h \
	${top_srcdir}/inc/zutils/netlink/Command.h \
	${top_srcdir}/inc/zutils/netlink/Message.h \
	${top_srcdir}/inc/zutils/netlink/GenericMessage.h \
	${top_srcdir}/inc/zutils/netlink/RouteMessage.h \
	${top_srcdir}/inc/zutils/netlink/Handler.h \
	${top_srcdir}/inc/zutils/netlink/Socket.h \
	${top_srcdir}/inc/zutils/netlink/GenericSocket.h \
	${top_srcdir}/inc/zutils/netlink/RouteLink.h \
	${top_srcdir}/inc/zutils/netlink/RouteLinkEvent.h \
	${top_srcdir}/inc/zutils/netlink/RouteSocket.h \
	${top_srcdir}/inc/zutils/netlink/ListLinksCommand.h \
	${top_srcdir}/inc/zutils/netlink/GetLinkCommand.h \
	${top_srcdir}/inc/zutils/netlink/SetLinkCommand.h \
	${top_srcdir}/inc/zutils/netlink/NewLinkCommand.h \
	${top_srcdir}/inc/zutils/netlink/DelLinkCommand.h

libnetlink_la_SOURCES = \
	Command.cpp \
	Message.cpp \
	GenericMessage.cpp \
	RouteMessage.cpp \
	Handler.cpp \
	Socket.cpp \
	GenericSocket.cpp \
	RouteLink.cpp \
	RouteLinkEvent.cpp \
	RouteSocket.cpp \
	ListLinksCommand.cpp \
	GetLinkCommand.cpp \
	SetLinkCommand.cpp \
	NewLinkCommand.cpp \
	DelLinkCommand.cpp
	