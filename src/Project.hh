
#ifndef OCTETOS_SCHEDULE_PROYECT_HH
#define OCTETOS_SCHEDULE_PROYECT_HH

#include <string>

#include "schedule.hh"
#include "Packing.hh"

namespace sche
{

class Temporary
{
public:
	const std::filesystem::path& create_directory();
	//const std::filesystem::path& create_document();
	const std::filesystem::path& create_document(std::ofstream&);
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
	bool create();
	bool create(const std::filesystem::path& destino);
	bool create_default_config();
	bool save(const std::filesystem::path&);
	
public:
	oct::ec::sche::Configuration* ep_config;
	
private:
	Temporary project_filename_temp;
	
};


}

#endif
