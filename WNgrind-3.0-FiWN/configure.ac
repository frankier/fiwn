#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(grind, 3.0, [wordnet@princeton.edu], WNgrind)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])

AC_PREFIX_DEFAULT([/usr/local/WordNet-3.0])

# Checks for programs.
AC_PROG_CC
AC_PROG_LEX
AC_PROG_YACC
AC_PROG_INSTALL

# Figure out if we're running flex
AC_MSG_RESULT([$LEX])
AM_CONDITIONAL(LEX_TEST, test x$LEX = xflex)

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([malloc.h string.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([strchr strdup])

AM_INIT_AUTOMAKE

# This doesn't do anything
AC_CONFIG_COMMANDS([default])

AC_CONFIG_FILES([Makefile
		dict/Makefile
		dict/dbfiles/Makefile
		doc/Makefile
		doc/html/Makefile
		doc/man/Makefile
		doc/pdf/Makefile
		doc/ps/Makefile
		src/Makefile
		src/grind/Makefile])

AC_OUTPUT

AC_MSG_RESULT(
[
WordNet grinder is now configured

  Installation directory:               ${prefix}

  To build and install grind:

    make
    make install

    See INSTALL file for details and environment variables
    which may need to be set on your system.

])
