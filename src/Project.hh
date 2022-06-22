
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


#ifndef OCTETOS_SCHEDULE_PROYECT_HH
#define OCTETOS_SCHEDULE_PROYECT_HH

#include <string>

#include "schedule.hh"
#include "Packing.hh"

namespace sche
{

class OCTETOS_SCHEDULE_DECLSPCE_DLL Temporary
{
public:
	const std::filesystem::path& create_directory();
	//const std::filesystem::path& create_document();
	const std::filesystem::path& create_document(std::ofstream&);
	const std::filesystem::path& get_path()const;
private:
	std::filesystem::path path_temp;
};


class OCTETOS_SCHEDULE_DECLSPCE_DLL Project
{
public:
    Project();
    ~Project();

    const oct::ec::sche::Data& get_data()const;

	const oct::ec::sche::Data& open(const std::filesystem::path& project_filename);
	bool run();
	bool save(const std::filesystem::path& source,const std::filesystem::path& destino);
	bool create();
	bool create(const std::filesystem::path& destino);
	bool create_default_config();
	bool save(const std::filesystem::path&);

public:
	oct::ec::sche::Data data;

private:
	Temporary project_filename_temp;

};


}

#endif
