
#ifndef OCTETOS_SCHEDULE_PROYECT_HH
#define OCTETOS_SCHEDULE_PROYECT_HH

#include <string>

#include "schedule.hh"
#include "Packing.hh"

namespace sche
{

class Project
{
public:
    Project();
    ~Project();


	void open(const std::string& project_filename, const std::string& output_directory);
	void open(const std::string& project_filename, const std::string& output_directory,oct::ec::echo, unsigned int,bool create_session);
	bool run();

private:
    std::string project_filename;
    char *project_filename_temp;
    oct::ec::sche::Enviroment evprog;
};


}

#endif
