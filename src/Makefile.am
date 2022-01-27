

uidir = $(pkgdatadir)/ui
ui_DATA = schedule.ui


AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(OCTETOS_SCHEDULE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = schedule

schedule_SOURCES = \
	main.cc WndMain.cc Packing.cc

schedule_LDFLAGS = 

schedule_LDADD = $(OCTETOS_SCHEDULE_LIBS)




EXTRA_DIST = $(ui_DATA)

# Remove ui directory on uninstall
uninstall-local:
	-rm -r $(uidir)
	-rm -r $(pkgdatadir)

