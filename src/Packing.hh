

#ifndef OCTETOS_PACKING_HH
#define OCTETOS_PACKING_HH

#include <string>
#include <zip.h>
#include <filesystem>
#if defined(__GNUC__) && defined(__linux__)
    #include <octetos/core/shell.hh>
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))
    #include <shell.hh>
#else
    #error "Pltaforma desconocida"
#endif


namespace oct::pack
{


class Package
{
public:
	/**
	*\brief Si es un directerio, comprime su contenido en el archivo destino
	*/
	virtual void compress(const std::filesystem::path& source,const std::filesystem::path& dest) = 0;
	virtual void extract(const std::filesystem::path& source,const std::filesystem::path& dest) = 0;
};


class Zip : public Package
{
public:
	Zip();

	virtual void compress(const std::filesystem::path& source,const std::filesystem::path& dest);
	virtual void extract(const std::filesystem::path& source,const std::filesystem::path& dest);
	virtual void add(const std::filesystem::path& source);

private:
	void compres_walk_directory(const std::filesystem::path& source);

	zip_t* zipper;
	oct::core::Shell shell;
};



}

#endif
