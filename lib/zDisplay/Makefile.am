if COND_GCOV
CPPFLAGS_GCOV = -g3 -Og -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

if COND_DEBUG
CPPFLAGS_DEBUG = -DDEBUG
endif

AM_CPPFLAGS := $(com_incdir) $(BOOST_CPPFLAGS) $(CPPFLAGS_GCOV) $(CPPFLAGS_DEBUG)
AM_LDFLAGS := $(LDFLAGS_GCOV)

noinst_LTLIBRARIES = libzDisplay.la

libzDisplay_la_SOURCES = \
    DisplayBuffer.cpp \
    DisplayVar.cpp \
    DisplayPage.cpp \
    Display.cpp \
    DisplayCommand.cpp
