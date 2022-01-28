
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#if defined(__GNUC__) && defined(__linux__)
	#include <octetos/core/Exception.hh>
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
    #include <Exception.hh>
#else
    #error "Pltaforma desconocida"
#endif
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
	else add(source,source);

	zip_close(zipper);
	zipper = NULL;
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
			add(fn,dirp->d_name);
		}
	}
	closedir(dp);
}
void Zip::add(const std::string& source, const std::string& name)
{
	zip_source_t *zip_file = zip_source_file(zipper,source.c_str(),0,0);
	if(zip_file == NULL ) throw core::Exception("Fallo al agregar archivo a zip '" + source + "' : " + std::string(zip_strerror(zipper)),__FILE__,__LINE__);
	if(zip_file_add(zipper,name.c_str(),zip_file,ZIP_FL_ENC_RAW) < 0) throw core::Exception("Fallo al agregar archivo a zip '" + source + "' : " + std::string(zip_strerror(zipper)),__FILE__,__LINE__);
}

#define COPY_BUF_SIZE 2048
void Zip::extract(const std::string& source,const std::string& dest)
{
    int err;
    zipper = zip_open(source.c_str(), 0, &err);
    if(zipper == NULL)
	{
		zip_error_t ziperr;
		zip_error_init_with_code(&ziperr,err);
		throw core::Exception("Fallo al abrir el archivo '" + dest + "' : " + zip_error_strerror(&ziperr),__FILE__,__LINE__);
	}

	zip_int64_t  num_entries = zip_get_num_entries(zipper, 0);
	struct zip_stat file_stat;
	int file_fd, bytes_read;
	zip_file* file_zip;
	char copy_buf[COPY_BUF_SIZE];
	for(zip_int64_t i = 0; i < num_entries; i++)
    {
        if(zip_stat_index(zipper, i, 0, &file_stat)) throw core::Exception("Fallo al extraer el archivo '" + source + "'",__FILE__,__LINE__);
        if((file_stat.name[0] != '\0') && (file_stat.name[strlen(file_stat.name)-1] == '/'))
        {
            if(mkdir(file_stat.name) && (errno != EEXIST)) throw core::Exception("Fallo al extraer el archivo '" + source + "'",__FILE__,__LINE__);
            std::cout << "Directory : " << file_stat.name << std::endl;
            continue;
        }
        std::cout << "Document : " << file_stat.name << std::endl;
        /*if((file_fd = open(file_stat.name, O_CREAT | O_TRUNC | O_WRONLY, 0666)) == -1) throw core::Exception("Fallo al extraer el archivo '" + source + "' : " + zip_error_strerror(&ziperr),__FILE__,__LINE__);
        if((file_zip = zip_fopen_index(zipper, i, 0)) == NULL) throw core::Exception("Fallo al extraer el archivo '" + source + "' : " + zip_error_strerror(&ziperr),__FILE__,__LINE__);

        do
        {
            if((bytes_read = zip_fread(file_zip, copy_buf, COPY_BUF_SIZE)) == -1) throw core::Exception("Fallo al extraer el archivo '" + source + "' : " + zip_error_strerror(&ziperr),__FILE__,__LINE__);
            if(bytes_read > 0) write(file_zip, copy_buf, bytes_read);
        }
        while(bytes_read > 0);
        zip_fclose(file_zip);
        close(file_fd);*/
    }
    zip_close(zipper);
}

}
