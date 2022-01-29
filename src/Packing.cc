
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <filesystem>
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




Zip::Zip() : zipper(NULL)
{
}

void Zip::compress(const std::filesystem::path& source,const std::filesystem::path& dest)
{
	int err;
	zipper = zip_open(dest.string().c_str(),ZIP_CREATE|ZIP_EXCL,&err);

	if(zipper == NULL)
	{
		zip_error_t ziperr;
		zip_error_init_with_code(&ziperr,err);
		std::string msg = "Fallo al abrir el archivo zip '" ;
		msg += dest.string() + "' : " + zip_error_strerror(&ziperr);
		throw core::Exception(msg,__FILE__,__LINE__);
	}

	if(std::filesystem::exists(source))
	{
		if(std::filesystem::is_directory(source)) compres_walk_directory(source);
		else add(source);
	}
	else
	{
	    std::string msg = "No se encontro '";
	    msg += source.string() + "'";
		throw core::Exception(msg,__FILE__,__LINE__);
	}

	zip_close(zipper);
	zipper = NULL;
}
void Zip::compres_walk_directory(const std::filesystem::path& source)
{
    for (auto const& dir_entry : std::filesystem::directory_iterator{source})
	{
		if(std::filesystem::is_directory(dir_entry))
		{
			if(zip_dir_add(zipper,dir_entry.path().string().c_str(),ZIP_FL_ENC_RAW) < 0)
			{
			    std::string msg = "Fallo al agregar archivo a zip '";
			    msg += dir_entry.path().string() + "' : " + std::string(zip_strerror(zipper));
                throw core::Exception(msg,__FILE__,__LINE__);
            }
			compres_walk_directory(dir_entry);
		}
		else
		{
			add(dir_entry);
		}
	}
}
void Zip::add(const std::filesystem::path& source)
{
	zip_source_t *zip_file = zip_source_file(zipper,source.string().c_str(),0,0);
	if(zip_file == NULL )
    {
			    std::string msg = "Fallo al agregar archivo a zip '";
			    msg += source.string() + "' : " + std::string(zip_strerror(zipper));
                throw core::Exception(msg,__FILE__,__LINE__);
    }
	if(zip_file_add(zipper,source.filename().string().c_str(),zip_file,ZIP_FL_ENC_RAW) < 0)
    {
			    std::string msg = "Fallo al agregar archivo a zip '";
			    msg += ((const std::string&)source) + "' : " + std::string(zip_strerror(zipper));
                throw core::Exception(msg,__FILE__,__LINE__);
    }
}

#define COPY_BUF_SIZE 2048
void Zip::extract(const std::filesystem::path& source,const std::filesystem::path& dest)
{
    if(not std::filesystem::exists(dest)) throw core::Exception("No existe el directorio destino " + dest.string() ,__FILE__,__LINE__);
    //std::cout << "dest = " << dest << "\n";
    //std::cout << "source = " << source << "\n";

    int err;
    zipper = zip_open(source.string().c_str(), 0, &err);
    if(zipper == NULL)
	{
		zip_error_t ziperr;
		zip_error_init_with_code(&ziperr,err);
		throw core::Exception("Fallo al abrir el archivo '" + dest.string() + "' : " + zip_error_strerror(&ziperr),__FILE__,__LINE__);
	}

	zip_int64_t  num_entries = zip_get_num_entries(zipper, 0);
	//std::cout << "num_entries = " << num_entries << "\n" ;
	struct zip_stat file_stat;
	int file_fd, bytes_read;
	zip_file* file_zip;
	char copy_buf[COPY_BUF_SIZE];
	std::filesystem::path filename;
	for(zip_int64_t i = 0; i < num_entries; i++)
    {
        if(zip_stat_index(zipper, i, 0, &file_stat)) throw core::Exception("Fallo al extraer el archivo '" + source.string() + "'",__FILE__,__LINE__);
        //std::cout << "file_stat.name = " << file_stat.name << "\n" ;
        filename = dest / file_stat.name;
        if((file_stat.name[0] != '\0') && (file_stat.name[strlen(file_stat.name)-1] == '/'))
        {
            if(not std::filesystem::create_directory(filename)) throw core::Exception("Fallo al extraer el archivo '" + source.string() + "'",__FILE__,__LINE__);
            continue;
        }

		if((file_fd = open(filename.string().c_str(), O_CREAT | O_TRUNC | O_WRONLY,0666)) == -1)
		{
			zip_error_t ziperr;
			std::string msg = "Fallo al leer el elemento '";
			msg += file_stat.name;
			msg += "'";
			throw core::Exception(msg,__FILE__,__LINE__);
		}

        if((file_zip = zip_fopen_index(zipper, i, 0)) == NULL)
		{
			zip_error_t ziperr;
			std::string msg = "Fallo al leer el elemento '";
			msg += file_stat.name;
			msg += "' : ";
			msg += zip_strerror(zipper);
			throw core::Exception(msg,__FILE__,__LINE__);
		}

        do
        {
            if((bytes_read = zip_fread(file_zip, copy_buf, COPY_BUF_SIZE)) == -1)
            {
				zip_error_t ziperr;
				zip_error_init_with_code(&ziperr,err);
				std::string msg = "Fallo al escribir el archivo '" ;
				msg = msg + file_stat.name + "'";
				throw core::Exception(msg,__FILE__,__LINE__);
			}
            if(bytes_read > 0) write(file_fd, copy_buf, bytes_read);
        }
        while(bytes_read > 0);

        zip_fclose(file_zip);
        close(file_fd);
    }
    zip_close(zipper);
}

}
