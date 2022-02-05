


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

$(top_srcdir)/src/resources.c : $(top_srcdir)/src/resources.xml $(top_srcdir)/src/schedule.ui $(top_srcdir)/src/schedule.png
	glib-compile-resources --target=$(top_srcdir)/src/resources.c --generate-source $(top_srcdir)/src/resources.xml

$(top_srcdir)/src/schedule.png : $(top_srcdir)/src/schedule.svg
	rsvg-convert $(top_srcdir)/src/schedule.svg -w 256 -h 256 -f png -o $(top_srcdir)/src/schedule.png


EXTRA_DIST = schedule.ui schedule.png sche.desktop


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(pkgdatadir)

