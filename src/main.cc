
/*
 * main.cc
 * Copyright (C) 2021 Azael Reyes <azael.devel@gmail.com>
 *
 * octetos-schedule is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * octetos-schedule is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtkmm.h>
#include <iostream>

#if defined(__GNUG__) && defined(__linux__) && !defined(CODEBLOCKS)
    #include "config.h"
#elif defined(__GNUG__) && (defined(_WIN32) || defined(_WIN64))

#elif defined(__GNUG__) && defined(__linux__) && defined(CODEBLOCKS)

#else
	#error "Pltaforma desconocida"
#endif





/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/octetos_schedule.ui" */
#if defined(CODEBLOCK)
    #define UI_FILE "schedule.ui"
#else
    #define UI_FILE "src/schedule.ui"
#endif // defined


int
main (int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);

	//Load the Glade file and instiate its widgets:
	Glib::RefPtr<Gtk::Builder> builder;
	try
	{
		builder = Gtk::Builder::create_from_file(UI_FILE);
	}
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	Gtk::Window* main_win = 0;
	builder->get_widget("main", main_win);


	if (main_win)
	{
		kit.run(*main_win);
	}
	return 0;
}

