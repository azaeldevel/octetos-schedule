
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
#include <filesystem>

#if defined(__GNUC__) && defined(__linux__)
    #include <csignal>
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))

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


    if(argc == 1)
    {
        std::cout << "Eliga el modo de operacion :\n";
        std::cout << "\t\tec-schedule --enable-develop/--enable-user";
    }
    std::filesystem::path root_directory;
    std::filesystem::path schedule_directory;
    for(unsigned int i = 0; i < argc; i++)
    {
        if(strcmp(argv[i],"--enable-develop") == 0)
        {
            root_directory = std::filesystem::current_path();
            schedule_directory = root_directory / "tests/schedule";
        }
        else if(strcmp(argv[i],"--enable-user") == 0)
        {
            root_directory = std::getenv("USERPROFILE");
            schedule_directory = root_directory / "Desktop/schedule";
        }
        else
        {
            std::cout << "Opcion desconocida : " << argv[i] << "\n";
        }
    }

    std::cout << "Schedule Directory : " << schedule_directory << "\n";
	std::string strDay = std::to_string(oct::core::getDayID());
	std::string strTime = std::to_string(oct::core::getTimeID());
	std::string strid = strDay + "-" + strTime;
	std::string strlog = "logs/" + strid;
	std::filesystem::path log_directory = schedule_directory / strlog;

	std::filesystem::path project_directory = schedule_directory / "project";

	oct::ec::sche::Enviroment* sche = new oct::ec::sche::Enviroment(log_directory,project_directory,log_directory);
	sche->enableEcho(&echo,2);

	bool ret;
	ret = sche->run();

	delete sche;

	return ret? EXIT_SUCCESS : EXIT_FAILURE;
}


