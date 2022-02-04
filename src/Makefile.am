

schedir = $(pkgdatadir)/sche
sche_DATA = schedule.ui $(top_builddir)/src/schedule.png sche.desktop


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(OCTETOS_SCHEDULE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES = liboctetos-schedule.la

liboctetos_schedule_la_SOURCES = schedule.cc  schedule-data.cc schedule-eval.cc  Packing.cc  Project.cc
liboctetos_schedule_la_LDFLAGS = $(OCTETOS_SCHEDULE_CFLAGS) 
liboctetos_schedule_la_LIBADD = $(OCTETOS_SCHEDULE_LIBS)

bin_PROGRAMS = ec-schedule schedule

schedule_SOURCES = main.cc WndMain.cc $(top_builddir)/src/resources.c Analyzer.cc
schedule_LDFLAGS = $(OCTETOS_SCHEDULE_CFLAGS) 
schedule_LDADD = $(OCTETOS_SCHEDULE_LIBS) liboctetos-schedule.la

ec_schedule_SOURCES = ec-schedule.cc
ec_schedule_LDFLAGS = 
ec_schedule_LDADD = liboctetos-schedule.la

$(top_builddir)/src/resources.c : $(top_srcdir)/src/resources.xml $(top_srcdir)/src/schedule.ui
	glib-compile-resources --target=$(top_builddir)/src/resources.c --generate-source $(top_srcdir)/src/resources.xml

EXTRA_DIST = $(sche_DATA)


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(schedir)
	-rm -r $(pkgdatadir)

