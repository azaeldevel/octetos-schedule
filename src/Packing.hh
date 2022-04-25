
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


#ifndef OCTETOS_PACKING_HH
#define OCTETOS_PACKING_HH

#include <string>
#include <zip.h>
#include <filesystem>
#if EXPORTING_OCTETOS_SCHEDULE_DLL
	#if _MSC_VER
		#define OCTETOS_SCHEDULE_DECLSPCE_DLL __declspec(dllexport)
	#elif __GNUG__

	#endif
#elif IMPORTING_OCTETOS_SCHEDULE_DLL
	#if _MSC_VER
		#define OCTETOS_SCHEDULE_DECLSPCE_DLL __declspec(dllimport)
	#elif __GNUG__

	#endif
#else
	#define OCTETOS_SCHEDULE_DECLSPCE_DLL
#endif
#if defined(__linux__)
    #include <octetos/core/shell.hh>
#elif defined(_WIN32) || defined(_WIN64)
    //#include <core/src/shell.hh>
#else
    #error "Pltaforma desconocida"
#endif



namespace oct::pack
{


class OCTETOS_SCHEDULE_DECLSPCE_DLL Package
{
public:
	/**
	*\brief Si es un directerio, comprime su contenido en el archivo destino
	*/
	virtual bool compress(const std::filesystem::path& source,const std::filesystem::path& dest) = 0;
	virtual bool extract(const std::filesystem::path& source,const std::filesystem::path& dest) = 0;
};


class OCTETOS_SCHEDULE_DECLSPCE_DLL Zip : public Package
{
public:
	Zip();

	virtual bool compress(const std::filesystem::path& source,const std::filesystem::path& dest);
	virtual bool extract(const std::filesystem::path& source,const std::filesystem::path& dest);
	virtual void add(const std::filesystem::path& source);

private:
	void compres_walk_directory(const std::filesystem::path& source);

	zip_t* zipper;
	//oct::core::Shell shell;
};



}

#endif
