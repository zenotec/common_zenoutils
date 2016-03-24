if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(com_incdir) $(BOOST_CPPFLAGS) $(CPPFLAGS_GCOV)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzConf.la

libzConf_la_SOURCES = \
    ConfigurationConnector.cpp \
    Configuration.cpp \
    ConfigurationHandler.cpp
