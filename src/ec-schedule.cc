
/*
 * main.cc
 * Copyright (C) 2021 Azael Reyes <azael.devel@gmail.com>
 *
 * AE is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AE is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <fstream>
#include <string>
#include <csignal>


#if defined(__GNUC__) && defined(__linux__)
    #include <octetos/core/shell.hh>
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
    #include <shell.hh>
#else
    #error "Pltaforma desconocida"
#endif

#include "schedule.hh"
#include "Project.hh"

void echo(const char* text)
{
	std::cout << text;
}
int main(int argc, const char* argv[])
{
    #if defined(__GNUC__) && defined(__linux__)
	signal(SIGSEGV,oct::core::signal_segmentv);
	signal(SIGABRT,oct::core::signal_abort);
    #elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))

    #else
        #error "Pltaforma desconocida"
    #endif

    if(argc != 3)
    {
        std::cout << "Uso : ec-schedule archivo_proyecto directorio_salida\n";
        return EXIT_FAILURE;
    }

    sche::Project project;
    project.open(argv[1],argv[2],&echo,2,true);
    bool ret = project.run();

	return ret? EXIT_SUCCESS : EXIT_FAILURE;
}


