#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libzutils], [0.1], [kevin.mahoney@zenotec.net])
AM_INIT_AUTOMAKE(libzutils, 0.1)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_AWK
AC_PROG_CPP
AC_PROG_MKDIR_P
AC_PROG_LIBTOOL
AC_PROG_LN_S
AC_PROG_MAKE_SET

LT_INIT

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([rt], [clock_gettime])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([memset getpid sem_init strerror])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])
                 
# Boost checks
BOOST_REQUIRE([1.41.1])

AC_OUTPUT
