


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(OCTETOS_SCHEDULE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g


desktopdir = /usr/share/applications
desktop_DATA = sche.desktop

icondir = /usr/share/pixmaps/
icon_DATA = schedule.png

lib_LTLIBRARIES = liboctetos-schedule.la


liboctetos_schedule_la_SOURCES = schedule.cc  schedule-data.cc schedule-eval.cc  Packing.cc  Project.cc
liboctetos_schedule_la_LDFLAGS = $(OCTETOS_SCHEDULE_CFLAGS) 
liboctetos_schedule_la_LIBADD = $(OCTETOS_SCHEDULE_LIBS) 

bin_PROGRAMS = ec-schedule schedule

schedule_SOURCES = main.cc WndMain.cc resources.c Analyzer.cc Configuration.cc
schedule_LDFLAGS = $(OCTETOS_SCHEDULE_CFLAGS) 
schedule_LDADD = $(OCTETOS_SCHEDULE_LIBS) liboctetos-schedule.la

ec_schedule_SOURCES = ec-schedule.cc
ec_schedule_LDFLAGS = 
ec_schedule_LDADD = liboctetos-schedule.la

resources.c : $(top_srcdir)/src/resources.xml $(top_srcdir)/src/schedule.ui $(top_srcdir)/src/schedule.ico 
	glib-compile-resources --target=resources.c --generate-source $(top_srcdir)/src/resources.xml --sourcedir $(top_srcdir)/src/

$(top_srcdir)/src/schedule.png : $(top_srcdir)/src/schedule.svg
	rsvg-convert $(top_srcdir)/src/schedule.svg -w 256 -h 256 -f png -o $(top_srcdir)/src/schedule.png


EXTRA_DIST = $(ui_DATA) $(desktop_DATA)


# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(pkgdatadir)

