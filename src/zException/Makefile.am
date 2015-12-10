if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

AM_CPPFLAGS := $(CPPFLAGS_GCOV) $(com_incdir) $(BOOST_CPPFLAGS)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzException.la

libzException_la_SOURCES = \
    Exception.cpp

