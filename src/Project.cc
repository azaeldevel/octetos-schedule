
/*
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


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#if defined(__linux__)
    #include <octetos/core/Exception.hh>
    #define MK_TEMP_DIR mkdtemp
	#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <core/src/Exception.hh>
    #define MK_TEMP_DIR mktemp
	#include <io.h>
#else
    #error "Plataforma desconocida"
#endif

#include "Project.hh"


namespace sche
{

const std::filesystem::path& Temporary::create_directory()
{
	if(not path_temp.empty()) throw oct::core::Exception("El directorio temporal esta ocupa.",__FILE__,__LINE__);

	path_temp = std::filesystem::temp_directory_path()/"XXXXXX";
	char* temple_path_temp = (char*)malloc(path_temp.string().size() + 1);
	strcpy(temple_path_temp,path_temp.string().c_str());
	char *str_path_temp = MK_TEMP_DIR(temple_path_temp);
	if(not str_path_temp)
	{
        std::string msg = "Fallo la creacion de directorio temporal ";
        msg += path_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    path_temp = str_path_temp;
#if defined(_WIN32) || defined(_WIN64)
    std::filesystem::create_directory(path_temp);
#endif
    if(not std::filesystem::exists(path_temp))
    {
        std::string msg = "No se encontro el directorio temporal ";
        msg += path_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    free(str_path_temp);
    return path_temp;
}
/*const std::filesystem::path& Temporary::create_path()
{
	if(not path_temp.empty()) throw oct::core::Exception("El directorio temporal esta ocupa.",__FILE__,__LINE__);

	path_temp = std::filesystem::temp_directory_path()/"XXXXXX";
	char* temple_path_temp = (char*)malloc(path_temp.string().size() + 1);
	strcpy(temple_path_temp,path_temp.string().c_str());
	char *str_path_temp = tmpnam(temple_path_temp);
	if(not str_path_temp)
	{
        std::string msg = "Fallo la creacion de directorio temporal ";
        msg += path_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    path_temp = str_path_temp;
    free(str_path_temp);
    return path_temp;
}*/
const std::filesystem::path& Temporary::create_document(std::ofstream& out)
{
	if(not path_temp.empty()) throw oct::core::Exception("El directorio temporal esta ocupa.",__FILE__,__LINE__);

	path_temp = std::filesystem::temp_directory_path()/"XXXXXX";
	char* temple_path_temp = (char*)malloc(path_temp.string().size() + 1);
	strcpy(temple_path_temp,path_temp.string().c_str());
	char *str_path_temp = tmpnam(temple_path_temp);
	if(not str_path_temp)
	{
        std::string msg = "Fallo la creacion de directorio temporal ";
        msg += path_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    path_temp = str_path_temp;
    out.open(path_temp);
    if(not std::filesystem::exists(path_temp))
    {
        std::string msg = "No se encontro el directorio temporal ";
        msg += path_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    free(str_path_temp);
    return path_temp;
}
const std::filesystem::path& Temporary::get_path()const
{
	return path_temp;
}







Project::Project()
{
}
Project::~Project()
{
}


const std::filesystem::path& Project::open(const std::filesystem::path& project)
{
	//if(not project_filename_temp.get_path().empty()) throw oct::core::Exception("El directorio temporal esta ocupa.",__FILE__,__LINE__);

	//project_filename_temp.create_directory();

	//oct::pack::Zip zip;
	//zip.extract(project,project_filename_temp.get_path());

    //return project_filename_temp.get_path();


    return project;
}

/*
bool Project::open(const std::filesystem::path& project, const std::filesystem::path& output,oct::ec::echo echo, unsigned int level,bool create_session)
{
    if(not open(project,output)) return false;
    evprog.enableEcho(echo,level);
    if(create_session) evprog.create_session();

    return true;
}
*/
/*
bool Project::run()
{
    return evprog.run();
}
*/
bool Project::save(const std::filesystem::path& source,const std::filesystem::path& destino)
{
    oct::pack::Zip zip;
	return zip.compress(source,destino);
}


bool Project::create()
{
	if(not create_default_config()) return false;

	return true;
}
bool Project::create_default_config()
{
	if(not project_filename_temp.get_path().empty()) throw oct::core::Exception("El directorio temporal esta ocupa.",__FILE__,__LINE__);

	project_filename_temp.create_directory();

	ep_config.set_schema_week(oct::ec::sche::Configuration::SchemaWeek::MF);
	ep_config.set_seconds_per_hour(std::time_t(2700));
	ep_config.set_out_directory("logs/schedule");
	ep_config.set_max_population(2000);
	ep_config.set_max_progenitor(300);
	ep_config.set_mutable_prob(0.05);
	ep_config.set_max_mutation(4);
	ep_config.set_junting_sigma(5.0);
		
	ep_config.write_file_project(project_filename_temp.get_path()/oct::ec::sche::Data::config_fn);
	
	return true;
}

bool Project::create(const std::filesystem::path& project)
{
	if(not create()) return false;

	//
	oct::pack::Zip zip;
	if(not zip.compress(project_filename_temp.get_path(),project)) throw oct::core::Exception("Fallo el proceso de compresion",__FILE__,__LINE__);

	return true;
}

bool Project::save(const std::filesystem::path& fn)
{
	return save(project_filename_temp.get_path(),fn);
}

}
