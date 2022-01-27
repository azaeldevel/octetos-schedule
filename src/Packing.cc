
#include <sys/stat.h>
#include <octetos/core/Exception.hh>
#include <dirent.h>
#include <string.h>
#include <iostream>

#include "Packing.hh"

namespace oct::pack
{


bool Package::is_directory(const std::string& fn)
{
	struct stat st;
	::stat(fn.c_str(),&st);
	
	return S_ISDIR(st.st_mode);
}













Zip::Zip() : zipper(NULL)
{
}

void Zip::compress(const std::string& source,const std::string& dest)
{
	int err;
	zipper = zip_open(dest.c_str(),ZIP_CREATE|ZIP_EXCL,&err);
	if(zipper == NULL)
	{
		zip_error_t ziperr;
		zip_error_init_with_code(&ziperr,err);
		throw core::Exception("Fallo al abrir el archivo '" + dest + "' : " + zip_error_strerror(&ziperr),__FILE__,__LINE__);
	}
	
	if(is_directory(source)) compres_walk_directory(source);
	else add(source);
	
	zip_close(zipper);
}
void Zip::compres_walk_directory(const std::string& source)
{
	DIR *dp = opendir(source.c_str());
	if(dp == NULL) throw core::Exception("Fallo al abrir el archivo de entrada '" + source + "' : " + std::to_string(errno),__FILE__,__LINE__);
	
	struct dirent *dirp;
	std::string fn;
	while((dirp = readdir(dp)) != NULL)
	{
		if(strcmp(dirp->d_name,".") == 0) continue;
		if(strcmp(dirp->d_name,"..") == 0) continue;
			
		fn = source + "/" + dirp->d_name;
		if(is_directory(fn))
		{
			if(zip_dir_add(zipper,dirp->d_name,ZIP_FL_ENC_RAW) < 0) throw core::Exception("Fallo al agregar archivo a zip '" + source + "' : " + std::string(zip_strerror(zipper)),__FILE__,__LINE__);
			compres_walk_directory(fn);
		}
		else
		{
			add(fn);
		}
	}
	closedir(dp);
}
void Zip::add(const std::string& source)
{
	zip_source_t *zip_file = zip_source_file(zipper,source.c_str(),0,0);
	if(zip_file == NULL ) throw core::Exception("Fallo al agregar archivo a zip '" + source + "' : " + std::string(zip_strerror(zipper)),__FILE__,__LINE__);
	if(zip_file_add(zipper,source.c_str(),zip_file,ZIP_FL_ENC_RAW) < 0) throw core::Exception("Fallo al agregar archivo a zip '" + source + "' : " + std::string(zip_strerror(zipper)),__FILE__,__LINE__);
}

void Zip::extract(const std::string& source,const std::string& dest)
{
	
}

}