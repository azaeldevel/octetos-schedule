
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
#include <string.h>
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
enum Mode
{
	UNKNOW,
	DEVELOP,
	USER,
	COMMAND
};
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
        std::cout << "\t\tec-schedule --develop|--local-processes|--project-file";
        return EXIT_FAILURE;
    }
    Mode mode = Mode::UNKNOW;
    sche::Project* project;
    std::filesystem::path root_directory;
    std::filesystem::path schedule_directory;
    std::filesystem::path log_directory;
    std::filesystem::path project_directory;
    std::filesystem::path project_file;
    for(unsigned int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"--develop") == 0)
        {
            root_directory = std::filesystem::current_path();
            schedule_directory = root_directory / "tests/schedule";
            mode = Mode::DEVELOP;
        }
        else if(strcmp(argv[i],"--local-processes") == 0)
        {
            root_directory = std::getenv("USERPROFILE");
            schedule_directory = root_directory / "Desktop/schedule";
            mode = Mode::USER;
        }
        else if(strcmp(argv[i],"--project-file") == 0)
        {
            project_file = argv[++i];
            mode = Mode::COMMAND;
        }
        else if(strcmp(argv[i],"--out-directory") == 0)
        {
            log_directory = argv[++i];
        }
        else
        {
            std::cout << "Opcion desconocida : " << argv[i] << "\n";
        }
    }

    if(mode == Mode::USER or mode == Mode::DEVELOP)
    {
        std::cout << "Schedule Directory : " << schedule_directory << "\n";
		std::string strDay = std::to_string(oct::core::getDayID());
		std::string strTime = std::to_string(oct::core::getTimeID());
		std::string strid = strDay + "-" + strTime;
		std::string strlog = "logs/" + strid;
		log_directory = schedule_directory / strlog;
		project_directory = schedule_directory / "project";
    }
    else if(mode == Mode::COMMAND)
    {
    	if(log_directory.empty()) throw oct::core::Exception("Se deve asignar un directory de resultado '--out-directory'",__FILE__,__LINE__);
	  	project = new sche::Project();
	  	project_directory = project->open(project_file);
  	}

	oct::ec::sche::Enviroment* sche = new oct::ec::sche::Enviroment(log_directory,project_directory,log_directory);
	sche->enableEcho(&echo,2);

	bool ret = sche->run();

	if(mode == Mode::COMMAND) delete project;
	delete sche;

	return ret? EXIT_SUCCESS : EXIT_FAILURE;
}


