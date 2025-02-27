dnl Process this file with autoconf to produce a configure script.

AC_INIT([nload], [0.7.4], [feedback@roland-riegel.de])

AC_CANONICAL_HOST

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([
Makefile
nload.spec
docs/Makefile
docs/nload.1
src/Makefile
])

AM_INIT_AUTOMAKE([foreign])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL

dnl System dependant jobs
AM_CONDITIONAL(HAVE_LINUX, false)
AM_CONDITIONAL(HAVE_BSD, false)
AM_CONDITIONAL(HAVE_DARWIN, false)
AM_CONDITIONAL(HAVE_SOLARIS, false)
AM_CONDITIONAL(HAVE_HPUX, false)
case $host_os in
    *linux*)
        AC_DEFINE(HAVE_LINUX, 1, [Define to 1 if your build target is Linux.])
        AM_CONDITIONAL(HAVE_LINUX, true)

        AC_CHECK_FUNCS([memset])
        ;;
    *bsd*)
        AC_DEFINE(HAVE_BSD, 1, [Define to 1 if your build target is BSD or Darwin.])
        AM_CONDITIONAL(HAVE_BSD, true)
        
        AC_FUNC_MALLOC
        ;;
    *darwin*)
        AC_DEFINE(HAVE_BSD, 1, [Define to 1 if your build target is BSD or Darwin.])
        AC_DEFINE(HAVE_DARWIN, 1, [Define to 1 if your build target is Darwin.])
        AM_CONDITIONAL(HAVE_BSD, true)
        AM_CONDITIONAL(HAVE_DARWIN, true)

        AC_FUNC_MALLOC
        ;;
    *solaris*)
        AC_DEFINE(HAVE_SOLARIS, 1, [Define to 1 if your build target is Solaris.])
        AM_CONDITIONAL(HAVE_SOLARIS, true)
        
        AC_CHECK_LIB(rt, main)
        AC_CHECK_LIB(kstat, kstat_open)
        AC_CHECK_LIB(socket, main)
        AC_CHECK_LIB(nsl, main)
        
        INCLUDES="${INCLUDES} -I/usr/local/include/ncurses -I/usr/include/ncurses"
        LIBS="${LIBS} -L/usr/gnu/lib"
        AC_SUBST(INCLUDES)
        ;;
    *hpux*)
        AC_DEFINE(HAVE_HPUX, 1, [Define to 1 if your build target is HP-UX.])
        AM_CONDITIONAL(HAVE_HPUX, true)
        
    	INCLUDES="${INCLUDES} -I/usr/local/include/ncurses"
    	LIBS="-L/usr/local/lib ${LIBS}"
    	AC_SUBST(INCLUDES)
        ;;
    *)
        AC_MSG_ERROR([nload does not support this operating system!])
        ;;
esac

dnl Checks for libraries.
AC_CHECK_LIB(ncurses, initscr)
AC_CHECK_LIB(form, new_form)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h curses.h form.h ncurses/form.h math.h sys/param.h sys/socket.h sys/time.h sys/ioctl.h arpa/inet.h netinet/in.h])

if test "$ac_cv_lib_ncurses_initscr" != "yes"; then
    AC_MSG_ERROR([ncurses library not found. ncurses is required for $PACKAGE_NAME.])
fi
if test "$ac_cv_header_curses_h" != "yes"; then
    AC_MSG_ERROR([ncurses development files not found. ncurses is required for $PACKAGE_NAME.])
fi
if test "$ac_cv_lib_form_new_form" != "yes"; then
    AC_MSG_ERROR([form library not found. This is part of ncurses which is required for $PACKAGE_NAME.])
fi
if test "$ac_cv_header_form_h $ac_cv_header_ncurses_form_h" = "no no"; then
    AC_MSG_ERROR([form development files not found. These are part of ncurses which is required for $PACKAGE_NAME.])
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Use some gcc specific options.
if test "$GCC" = "yes" ; then
     CFLAGS="$CFLAGS -Wall"
     CXXFLAGS="$CXXFLAGS -Wall"
fi

dnl Forward sysconfdir to the preprocessor
CPPFLAGS="$CPPFLAGS -DSYSCONFDIR='\"\$(sysconfdir)\"'"

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gettimeofday inet_ntoa socket])

AC_OUTPUT
