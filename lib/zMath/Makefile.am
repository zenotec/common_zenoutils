if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

if COND_DEBUG
CPPFLAGS_DEBUG = -DDEBUG
endif

AM_CPPFLAGS := $(com_incdir) $(BOOST_CPPFLAGS) $(CPPFLAGS_GCOV) $(CPPFLAGS_DEBUG)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzMath.la

libzMath_la_SOURCES = \
    Sum.cpp \
    Mean.cpp \
    Variance.cpp

