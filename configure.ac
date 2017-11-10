#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([libzutils], [1.1], [kevin.mahoney@zenotec.net])
AM_INIT_AUTOMAKE([foreign parallel-tests color-tests])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_LANG_CPLUSPLUS
AX_CXX_COMPILE_STDCXX_11

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

#########################################################################################

#########################################################################################

AC_ARG_ENABLE([zmessage],
  [AS_HELP_STRING([--disable-zmessage],
    [disable building zMessage subsystem])],
    [],
    [enable_zmessage=yes])
AM_CONDITIONAL([COND_ZSOCKET],[test "$enable_zmessage" = yes])

#########################################################################################

AC_ARG_ENABLE([zsocket],
  [AS_HELP_STRING([--disable-zsocket],
    [disable building zSocket subsystem])],
    [],
    [enable_zsocket=yes])
AM_CONDITIONAL([COND_ZSOCKET],[test "$enable_zsocket" = yes])

#########################################################################################

AC_ARG_ENABLE([zinterface],
  [AS_HELP_STRING([--disable-zinterface],
    [disable building zInterface subsystem])],
    [],
    [enable_zinterface=yes])
AM_CONDITIONAL([COND_ZINTERFACE],[test "$enable_zinterface" = yes])

#########################################################################################

AC_ARG_ENABLE([zserial],
  [AS_HELP_STRING([--disable-zserial],
    [disable building zSerial subsystem])],
    [],
    [enable_zserial=yes])
AM_CONDITIONAL([COND_ZSERIAL],[test "$enable_zserial" = yes])

#########################################################################################

AC_ARG_ENABLE([zled],
  [AS_HELP_STRING([--disable-zled],
    [disable building zLed subsystem])],
    [],
    [enable_zled=yes])
AM_CONDITIONAL([COND_ZLED],[test "$enable_zled" = yes])

#########################################################################################

AC_ARG_ENABLE([zgpio],
  [AS_HELP_STRING([--disable-zgpio],
    [disable building zGpio subsystem])],
    [],
    [enable_zgpio=yes])
AM_CONDITIONAL([COND_ZGPIO],[test "$enable_zgpio" = yes])

#########################################################################################

AC_ARG_ENABLE([zprogram],
  [AS_HELP_STRING([--disable-zprogram],
    [disable building zProgram subsystem])],
    [],
    [enable_zprogram=yes])
AM_CONDITIONAL([COND_ZPROGRAM],[test "$enable_zprogram" = yes])

#########################################################################################

AC_ARG_ENABLE([ztimer],
  [AS_HELP_STRING([--disable-ztimer],
    [disable building zTimer subsystem])],
    [],
    [enable_ztimer=yes])
AM_CONDITIONAL([COND_ZTIMER],[test "$enable_ztimer" = yes])

#########################################################################################

AC_ARG_ENABLE([zthread],
  [AS_HELP_STRING([--disable-zthread],
    [disable building zThread subsystem])],
    [],
    [enable_zthread=yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zmessage" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zsocket" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zinterface" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zserial" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zled" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zgpio" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_ztimer" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zprogram" = yes])
AM_CONDITIONAL([COND_ZTHREAD],[test "$enable_zthread" = yes])

#########################################################################################

AC_ARG_ENABLE([zsignal],
  [AS_HELP_STRING([--disable-zsignal],
    [disable building zSignal subsystem])],
    [],
    [enable_zsignal=yes])
AM_CONDITIONAL([COND_ZSIGNAL],[test "$enable_zprogram" = yes])
AM_CONDITIONAL([COND_ZSIGNAL],[test "$enable_ztimer" = yes])
AM_CONDITIONAL([COND_ZSIGNAL],[test "$enable_zthread" = yes])
AM_CONDITIONAL([COND_ZSIGNAL],[test "$enable_zsignal" = yes])

#########################################################################################

AC_ARG_ENABLE([zconfig],
  [AS_HELP_STRING([--disable-zconfig],
    [disable building zConfig subsystem])],
    [],
    [enable_zconfig=yes])
AM_CONDITIONAL([COND_ZCONFIG],[test "$enable_zinterface" = yes])
AM_CONDITIONAL([COND_ZCONFIG],[test "$enable_zserial" = yes])
AM_CONDITIONAL([COND_ZCONFIG],[test "$enable_zled" = yes])
AM_CONDITIONAL([COND_ZCONFIG],[test "$enable_zgpio" = yes])
AM_CONDITIONAL([COND_ZCONFIG],[test "$enable_zconfig" = yes])

#########################################################################################

AC_ARG_ENABLE([zevent],
  [AS_HELP_STRING([--disable-zevent],
    [disable building zEvent subsystem])],
    [],
    [enable_zevent=yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zinterface" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zserial" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zled" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zgpio" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zprogram" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_ztimer" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zthread" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zsignal" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zconfig" = yes])
AM_CONDITIONAL([COND_ZEVENT],[test "$enable_zevent" = yes])

#########################################################################################

AC_ARG_ENABLE([zmath],
  [AS_HELP_STRING([--disable-zmath],
    [disable building zMath subsystem])],
    [],
    [enable_zmath=yes])
AM_CONDITIONAL([COND_ZMATH],[test "$enable_zmath" = yes])

#########################################################################################

AC_ARG_ENABLE([zdata],
  [AS_HELP_STRING([--disable-zdata],
    [disable building zData subsystem])],
    [],
    [enable_zdata=yes])
AM_CONDITIONAL([COND_ZDATA],[test "$enable_zconfig" = yes])
AM_CONDITIONAL([COND_ZDATA],[test "$enable_zdata" = yes])

#########################################################################################

AC_ARG_ENABLE([zsem],
  [AS_HELP_STRING([--disable-zsem],
    [disable building zSem subsystem])],
    [],
    [enable_zsem=yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zmessage" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zsocket" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zinterface" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zserial" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zled" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zgpio" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zprogram" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_ztimer" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zthread" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zevent" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zconfig" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zdata" = yes])
AM_CONDITIONAL([COND_ZSEM],[test "$enable_zsem" = yes])

#########################################################################################

AC_ARG_ENABLE([zlog],
  [AS_HELP_STRING([--disable-zlog],
    [disable building zLog subsystem])],
    [],
    [enable_zlog=yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zmessage" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zsocket" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zinterface" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zserial" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zled" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zgpio" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zprogram" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_ztimer" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zthread" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zsignal" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zevent" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zmath" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zconfig" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zdata" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zsem" = yes])
AM_CONDITIONAL([COND_ZLOG],[test "$enable_zlog" = yes])

#########################################################################################
#########################################################################################

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create], [], [AC_MSG_ERROR([Unable to find libpthread])])
AC_CHECK_LIB([rt], [clock_gettime], [], [AC_MSG_ERROR([Unable to find librt])])
AC_CHECK_LIB([uuid], [uuid_generate], [], [AC_MSG_ERROR([Unable to find libuuid])])

PKG_CHECK_MODULES([NL3], [libnl-3.0], [], [AC_MSG_ERROR([Unable to find libnl-3])])
PKG_CHECK_MODULES([NLGENL3], [libnl-genl-3.0], [], [AC_MSG_ERROR([Unable to find libnl-genl-3])])
PKG_CHECK_MODULES([NLRTNL3], [libnl-route-3.0], [], [AC_MSG_ERROR([Unable to find libnl-route-3])])

# Checks for header files.
AC_CHECK_HEADERS([stdio.h stdlib.h stddef.h stdint.h string.h unistd.h fcntl.h arpa/inet.h netinet/in.h sys/ioctl.h sys/socket.h sys/time.h termios.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_INT16_T
AC_TYPE_UINT16_T
AC_TYPE_INT32_T
AC_TYPE_UINT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset mkdir pow floor rmdir socket strtol strerror getpid])

# Common variables
com_incdir='-I${top_srcdir}/inc'
AC_SUBST([com_incdir])

num_cpus='$(grep -c ^processor /proc/cpuinfo)'
AC_SUBST([num_cpus])

AC_CONFIG_FILES([
    Makefile
      lib/Makefile
        lib/zLog/Makefile
        lib/zSem/Makefile
        lib/zData/Makefile
        lib/zMath/Makefile
        lib/zEvent/Makefile
        lib/zConfig/Makefile
        lib/zSignal/Makefile
        lib/zThread/Makefile
        lib/zTimer/Makefile
        lib/zProgram/Makefile
        lib/zGpio/Makefile
        lib/zLed/Makefile
        lib/zSerial/Makefile
        lib/zInterface/Makefile
        	lib/zInterface/netlink/Makefile
        	lib/zInterface/nl80211/Makefile
        lib/zSocket/Makefile
        lib/zMessage/Makefile
        lib/zCommand/Makefile
        lib/zSwitch/Makefile
        lib/zThermo/Makefile
        lib/zDisplay/Makefile
      test/Makefile
        test/zLog/Makefile
        test/zSem/Makefile
        test/zMath/Makefile
        test/zEvent/Makefile
        test/zThread/Makefile
        test/zTimer/Makefile
        test/zProgram/Makefile
        test/zData/Makefile
        test/zQueue/Makefile
        test/zConfig/Makefile
        test/zGpio/Makefile
        test/zLed/Makefile
        test/zSerial/Makefile
        test/zInterface/Makefile
        test/zSocket/Makefile
        test/zMessage/Makefile
        test/zCommand/Makefile
        test/zSwitch/Makefile
        test/zThermo/Makefile
        test/zDisplay/Makefile
])

# Boost checks
BOOST_REQUIRE([1.41.1])

AC_OUTPUT
