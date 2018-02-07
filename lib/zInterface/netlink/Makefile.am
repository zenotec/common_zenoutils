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
	${top_srcdir}/inc/zutils/netlink/Attribute.h \
	${top_srcdir}/inc/zutils/netlink/Command.h \
	Command.cpp \
	${top_srcdir}/inc/zutils/netlink/Message.h \
	Message.cpp \
	${top_srcdir}/inc/zutils/netlink/GenericMessage.h \
	GenericMessage.cpp \
	${top_srcdir}/inc/zutils/netlink/RouteMessage.h \
	RouteMessage.cpp \
	${top_srcdir}/inc/zutils/netlink/Handler.h \
	Handler.cpp \
	${top_srcdir}/inc/zutils/netlink/Socket.h \
	Socket.cpp \
	${top_srcdir}/inc/zutils/netlink/GenericSocket.h \
	GenericSocket.cpp \
	${top_srcdir}/inc/zutils/netlink/RouteLink.h \
	RouteLink.cpp \
	${top_srcdir}/inc/zutils/netlink/RouteLinkEvent.h \
	RouteLinkEvent.cpp \
	${top_srcdir}/inc/zutils/netlink/RouteSocket.h \
	RouteSocket.cpp \
	${top_srcdir}/inc/zutils/netlink/ListLinksCommand.h \
	ListLinksCommand.cpp \
	${top_srcdir}/inc/zutils/netlink/GetLinkCommand.h \
	GetLinkCommand.cpp \
	${top_srcdir}/inc/zutils/netlink/SetLinkCommand.h \
	SetLinkCommand.cpp \
	${top_srcdir}/inc/zutils/netlink/NewLinkCommand.h \
	NewLinkCommand.cpp \
	${top_srcdir}/inc/zutils/netlink/DelLinkCommand.h \
	DelLinkCommand.cpp
	