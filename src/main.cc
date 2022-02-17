
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

#if defined(__linux__) && !defined(CODEBLOCK_IDE)
    #include "config.h"
#elif (defined(_WIN32) || defined(_WIN64))

#elif defined(__linux__) && defined(CODEBLOCK_IDE)

#else
	#error "Pltaforma desconocida"
#endif





/* For testing propose use the local (not installed) ui file */
/* #define UI_FILE PACKAGE_DATA_DIR"/ui/octetos_schedule.ui" */
#if defined(CODEBLOCK_IDE)
    #define UI_FILE "schedule.ui"
#else
    #define UI_FILE "src/schedule.ui"
#endif // defined

#include "Main.hh"

int main (int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);

	//Load the Glade file and instiate its widgets:
	Glib::RefPtr<Gtk::Builder> builder;
	try
	{
        builder = Gtk::Builder::create_from_resource("/sche/schedule.ui");
	}
	catch (const Glib::FileError & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch (const Gtk::BuilderError& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	//Login* wndLogin = 0;
	sche::Main* wndMain = 0;
	builder->get_widget_derived("wndMain", wndMain);

	try
	{
		if (wndMain)
		{
			kit.run(*wndMain);
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}

