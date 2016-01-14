#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libzutils], [0.1], [kevin.mahoney@zenotec.net])
AM_INIT_AUTOMAKE([foreign parallel-tests color-tests])
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

m4_ifndef([PKG_PROG_PKG_CONFIG],
  [m4_fatal([Could not locate the pkg-config autoconf
    macros. These are usually located in /usr/share/aclocal/pkg.m4.
    If your macros are in a different location, try setting the
    environment variable AL_OPTS="-I/other/macro/dir" before running
    ./autogen.sh or autoreconf again.])])
PKG_PROG_PKG_CONFIG

AC_ARG_ENABLE([valgrind],
  [AS_HELP_STRING([--enable-valgrind],
    [use Valgrind to test the test suite])],
    [],
    [enable_valgrind=no])
AM_CONDITIONAL([COND_VALGRIND],[test '!' "$enable_valgrind" = no])

AC_ARG_ENABLE([gcov],
  [AS_HELP_STRING([--enable-gcov],
    [use Gcov to test the test suite])],
    [],
    [enable_gcov=no])
AM_CONDITIONAL([COND_GCOV],[test '!' "$enable_gcov" = no])

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_ERROR([Unable to find libpthread])])
AC_CHECK_LIB([rt], [clock_gettime], [], [AC_MSG_ERROR([Unable to find librt])])
AC_CHECK_LIB([uuid], [uuid_generate], [], [AC_MSG_ERROR([Unable to find libuuid])])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_CHECK_FUNCS([memset getpid sem_init strerror])

# Common variables
com_incdir='-I${top_srcdir}/inc'
AC_SUBST([com_incdir])

AC_CONFIG_FILES([
    Makefile
      src/Makefile
        src/zCom/Makefile
        src/zCommand/Makefile
        src/zConf/Makefile
        src/zData/Makefile
        src/zEvent/Makefile
        src/zException/Makefile
        src/zGpio/Makefile
        src/zLed/Makefile
        src/zLog/Makefile
        src/zMessage/Makefile
        src/zNode/Makefile
        src/zProgOpt/Makefile
        src/zSem/Makefile
        src/zSocket/Makefile
        src/zSwitch/Makefile
        src/zThermo/Makefile
        src/zThread/Makefile
        src/zTimer/Makefile
      test/Makefile
        test/zCom/Makefile
        test/zCommand/Makefile
        test/zConf/Makefile
        test/zData/Makefile
        test/zEvent/Makefile
        test/zGpio/Makefile
        test/zLed/Makefile
        test/zLog/Makefile
        test/zMessage/Makefile
        test/zNode/Makefile
        test/zProgOpt/Makefile
        test/zQueue/Makefile
        test/zSem/Makefile
        test/zSocket/Makefile
        test/zSwitch/Makefile
        test/zThermo/Makefile
        test/zThread/Makefile
        test/zTimer/Makefile
])
                 
# Boost checks
BOOST_REQUIRE([1.41.1])

AC_OUTPUT
