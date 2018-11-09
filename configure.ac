AC_PREREQ([2.69])
AC_INIT([typhoeus-cpp], [1.0.0], [typhoeus-cpp@krugerheavyindustries.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
LT_INIT
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_CONFIG_HEADERS([src/config.h])
AC_CHECK_HEADERS([curl/curl.h])
AC_CHECK_LIB([crypto], [main])
AC_CHECK_LIB([ssl], [main])
AC_CHECK_LIB([curl], [main])
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  examples/Makefile
])
AC_CONFIG_SUBDIRS
AC_OUTPUT
