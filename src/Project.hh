
#ifndef OCTETOS_SCHEDULE_PROYECT_HH
#define OCTETOS_SCHEDULE_PROYECT_HH

#include <string>

#include "schedule.hh"
#include "Packing.hh"

namespace sche
{

class TemporaryDirectory
{
public:
	const std::filesystem::path& create();
	const std::filesystem::path& get_path()const;
private:
	std::filesystem::path path_temp;
};
class Project
{
public:
    Project();
    ~Project();


	const std::filesystem::path& open(const std::filesystem::path& project_filename);
	bool run();
	bool save(const std::filesystem::path& source,const std::filesystem::path& destino);
	bool create(const std::filesystem::path& destino);
	
private:
    //std::filesystem::path project_filename_temp;
	TemporaryDirectory project_filename_temp;
};


}

#endif
