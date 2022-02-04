

AC_INIT(octetos-schedule, 0.12.5-alpha, azael.devel@gmail.com)

AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE


AM_SILENT_RULES([yes])

AC_PROG_CXX




desktopdir = $(datadir)/$(PACKAGE_NAME)
desktop_in_files = src/sche.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@




LT_INIT

dnl TODO: mover zip a su propio projecto
PKG_CHECK_MODULES(OCTETOS_SCHEDULE, [octetos-core gtkmm-3.0 libzip octetos-EC])


AM_CONDITIONAL(WITH_TESTS, test x"$with_tests" = "xyes")
AC_ARG_WITH([tests], AS_HELP_STRING([--with-tests], [Compilando con soporte para Tests]))
if test "x$with_tests" = "xyes"; then 
	PKG_CHECK_MODULES(CUNIT,cunit)
fi

AC_CONFIG_FILES(src/sche.desktop)

AC_OUTPUT([
Makefile
src/Makefile
tests/Makefile

])
