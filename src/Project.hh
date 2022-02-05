
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


	std::filesystem::path open(const std::filesystem::path& project_filename);
	bool run();
	bool save(const std::filesystem::path& source,const std::filesystem::path& destino);
	
private:
    std::filesystem::path project_filename_temp;
};


}

#endif
