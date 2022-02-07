
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#if defined(__GNUC__) && defined(__linux__)
    #include <octetos/core/Exception.hh>
    #define MK_TEMP_DIR mkdtemp
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
    #include <Exception.hh>
    #define MK_TEMP_DIR mktemp
#else
    #error "Pltaforma desconocida"
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
    std::filesystem::create_directory(project_filename_temp);
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
const std::filesystem::path& Temporary::create_document()
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
}
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
	project_filename_temp.create_directory();

	oct::pack::Zip zip;
	zip.extract(project,project_filename_temp.get_path());
	/*
	for (auto const& dir_entry : std::filesystem::directory_iterator{project_filename_temp})
    {
        std::cout << "project_filename_temp : " << dir_entry << "\n";
    }
    */

    //evprog.init(output,project_filename_temp,output);

    return project_filename_temp.get_path();
}

/*bool Project::open(const std::filesystem::path& project, const std::filesystem::path& output,oct::ec::echo echo, unsigned int level,bool create_session)
{
    if(not open(project,output)) return false;
    evprog.enableEcho(echo,level);
    if(create_session) evprog.create_session();

    return true;
}*/
/*bool Project::run()
{
    return evprog.run();
}*/
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
	
	std::ofstream file;
	file.open(project_filename_temp.get_path()/oct::ec::sche::Data::config_fn);
	
	file << "schema_week = \"MF\";\n";
	file << "seconds = 2700;\n";
	file << "out = \"logs/schedule\";\n";
	file << "max_population = 1000;\n";
	file << "max_progenitor = 100;\n";
	file << "mutable_prob = 0.05;\n";
	file << "max_mutation = 4;\n";
	file.flush();
	file.close();
		
	return true;
}

bool Project::create(const std::filesystem::path& project)
{
	if(not create()) return false;
	
	//
	//Temporary temp_zip;
	//std::ofstream file;
	//temp_zip.create_document(file);	
	//std::cout << "Path : " << temp_zip.get_path() << "\n";
	//if(not file.is_open()) throw oct::core::Exception("Archivo de trabajo no esta abierto",__FILE__,__LINE__);
	
	//uardar project
	oct::pack::Zip zip;
	if(not zip.compress(project_filename_temp.get_path(),project)) throw oct::core::Exception("Fallo el proceso de compresion",__FILE__,__LINE__);


	
	return true;
}

}
