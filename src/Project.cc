
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
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


Project::Project()
{
}
Project::~Project()
{
}


bool Project::open(const std::filesystem::path& project, const std::filesystem::path& output)
{
	project_filename_temp = std::filesystem::temp_directory_path()/"XXXXXX";
	char* temple_project_filename_temp = (char*)malloc(project_filename_temp.string().size() + 1);
	strcpy(temple_project_filename_temp,project_filename_temp.string().c_str());
	char *str_project_filename_temp = MK_TEMP_DIR(temple_project_filename_temp);
	if(not str_project_filename_temp)
	{
        std::string msg = "Fallo la creacion de directorio temporal ";
        msg += project_filename_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    project_filename_temp = str_project_filename_temp;
#if defined(_WIN32) || defined(_WIN64)
    std::filesystem::create_directory(project_filename_temp);
#endif
    if(not std::filesystem::exists(project_filename_temp))
    {
        std::string msg = "No se encontro el directorio temporal ";
        msg += project_filename_temp.string();
        throw oct::core::Exception(msg,__FILE__,__LINE__);
    }
    free(temple_project_filename_temp);

	oct::pack::Zip zip;
	zip.extract(project,project_filename_temp);
	/*for (auto const& dir_entry : std::filesystem::directory_iterator{project_filename_temp})
    {
        std::cout << "project_filename_temp : " << dir_entry << "\n";
    }*/

    evprog.init(output,project_filename_temp,output);

    return true;
}
bool Project::open(const std::filesystem::path& project, const std::filesystem::path& output,oct::ec::echo echo, unsigned int level,bool create_session)
{
    if(not open(project,output)) return false;
    evprog.enableEcho(echo,level);
    if(create_session) evprog.create_session();

    return true;
}
bool Project::run()
{
    return evprog.run();
}
bool Project::save(const std::filesystem::path& source,const std::filesystem::path& destino)
{
    oct::pack::Zip zip;
	return zip.compress(source,destino);
}

}
