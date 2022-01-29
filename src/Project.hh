
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


	bool open(const std::filesystem::path& project_filename, const std::filesystem::path& output_directory);
	bool open(const std::filesystem::path& project_filename, const std::filesystem::path& output_directory,oct::ec::echo, unsigned int,bool create_session);
	bool run();
	bool save(const std::filesystem::path& source,const std::filesystem::path& destino);

private:
    std::filesystem::path project_filename;
    std::filesystem::path project_filename_temp;
    oct::ec::sche::Enviroment evprog;
};


}

#endif
