
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "Project.hh"


namespace sche
{


Project::Project() : project_filename_temp(NULL)
{
}
Project::~Project()
{
    if(not project_filename_temp)
    {
        rmdir(project_filename_temp);
        project_filename_temp = NULL;
    }
}


void Project::open(const std::string& project, const std::string& output)
{
    char tmpfn[] = "/tmp/tmpdir.XXXXXX";
    project_filename_temp = mktemp(tmpfn);
    if(project_filename_temp == NULL) throw oct::core::Exception("Fallo la apertura del archivo de projecto",__FILE__,__LINE__);

	oct::pack::Zip zip;
	zip.extract(project,project_filename_temp);

    evprog.init(output,project_filename_temp,output);
}
void Project::open(const std::string& project, const std::string& output,oct::ec::echo echo, unsigned int level,bool create_session)
{
    open(project,output);
    evprog.enableEcho(echo,level);
    if(create_session) evprog.create_session();
}
bool Project::run()
{
    return evprog.run();
}

}
