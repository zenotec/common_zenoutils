export MAKEFLAGS="-j $(grep -c ^processor /proc/cpuinfo)"

if COND_GCOV
CPPFLAGS_GCOV = -g -O0 -fprofile-arcs -ftest-coverage --no-inline
LDFLAGS_GCOV = -lgcov
endif

if COND_DEBUG
CPPFLAGS_DEBUG = -DDEBUG
endif

AM_CPPFLAGS := \
  $(CPPFLAGS_GCOV) \
  $(CPPFLAGS_DEBUG) \
  $(com_incdir) \
  $(OPENSSL_INCLUDES) 

AM_LDFLAGS := \
	$(LDFLAGS_GCOV) \
	$(OPENSSL_LDFLAGS)
	
libzSecurity_la_CPPFLAGS = \
	$(AM_CPPFLAGS)
	
libzSecurity_la_LDFLAGS = \
	$(AM_LIBS) \
	$(OPENSSL_LIBS)
	
libzSecurity_la_LIBFLAGS = \
	$(OPENSSL_LIBS)

noinst_LTLIBRARIES = libzSecurity.la

libzSecurity_la_SOURCES = \
	SecurityWpa.cpp \
	Crypto.cpp
	
