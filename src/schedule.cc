/*
 * main.cc
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

#include <sstream>
#include <fstream>
#include <iostream>
#include <locale>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "schedule.hh"

#ifdef _WIN32 || _WIN64

extern "C" char* strptime(const char* s,
	const char* f,
	struct tm* tm) {
	// Isn't the C++ standard lib nice? std::get_time is defined such that its
	// format parameters are the exact same as strptime. Of course, we have to
	// create a string stream first, and imbue it with the current C locale, and
	// we also have to make sure we return the right things if it fails, or
	// if it succeeds, but this is still far simpler an implementation than any
	// of the versions in any of the C standard libraries.
	std::istringstream input(s);
	input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
	input >> std::get_time(tm, f);
	if (input.fail()) {
		return nullptr;
	}
	return (char*)(s + input.tellg());
}

#endif

namespace oct::core
{
	Person::Person(const std::string& name,const std::string& ap,const std::string& am)
	{
		
	}
	Person::Person(const std::string& name)
	{
		std::stringstream ss(name);

		
	}
	Person::Person()
	{
	}
	 
	const Person& Person::operator =(const std::string& name)
	{
		

		return *this;
	}
}

namespace oct::sche
{

	Teacher::Teacher(const std::string& name,const std::string& ap,const std::string& am) : oct::core::Person(name,ap,am)
	{
		
	}
	Teacher::Teacher(const std::string& name) : oct::core::Person(name)
	{
		
	}
	Teacher::Teacher()
	{
		
	}

	
	Room::Room(const std::string& name)
	{
		
	}
	Room::Room()
	{
		
	}

	
	Subject::Subject(const std::string& name)
	{
		
	}
	Subject::Subject()
	{
		
	}


	
	Teachers::Row::Row()
	{
	}
	Teachers::Row::Row(int z) : std::vector<Teachers::Time>(z)
	{
	}
	Teachers::Teachers(const std::string& fn)
	{
		loadFile(fn);
	}
	bool Teachers::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data,strTime;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				while(std::getline(str,data,','))
				{
					Teachers::Row row;
					//std::cout << data << ",";
					row.teacher = data;
					Time time;					
					row.push_back(time);
					std::stringstream ssTime(data);
					std::getline(ssTime,strTime,'-');
					strptime(strTime.c_str(), "%H:%M",&time.begin);
					std::getline(ssTime,strTime,'-');
					strptime(strTime.c_str(), "%H:%M",&time.end);
					//std::cout << std::put_time(&time.begin, "%H:%M");
					std::cout << "-";
					//std::cout << std::put_time(&time.end, "%H:%M");
					//std::cout << ",";
					teachers.push_back(row);
				}
				//std::cout << "\n";
			}
		}		
		
		return true;
	}



	
	
	/*Subjects::Row::Row()
	{
		
	}*/
	Subjects::Subjects(const std::string& fn)
	{
		loadFile(fn);
	}
	bool Subjects::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				//std::cout << line;
				std::getline(str,data,',');
				//Subjects::Row row;
				
				//std::cout << "\n";
			}
		}
		
		return true;
	}

	
	Rooms::Row::Row()
	{
	}
	Rooms::Row::Row(int z) : std::vector<Rooms::Time>(z)
	{
	}
	Rooms::Rooms(const std::string& fn)
	{
		loadFile(fn);
	}
	bool Rooms::loadFile(const std::string& fn)
	{
		std::fstream csv(fn, std::ios::in);
		std::string line,data,strTime;
		if(csv.is_open())
		{
			while(std::getline(csv,line))
			{
				std::stringstream str(line);
				std::getline(str,data,',');
				Rooms::Row row;
				std::cout << data << ",";
				row.room = data;
				Time time;			
				row.push_back(time);
				std::stringstream ssTime(data);
					std::getline(ssTime,strTime,'-');
					strptime(strTime.c_str(), "%H:%M",&time.begin);
					std::getline(ssTime,strTime,'-');
					strptime(strTime.c_str(), "%H:%M",&time.end);
					std::cout << std::put_time(&time.begin, "%H:%M");
					std::cout << "-";
					std::cout << std::put_time(&time.end, "%H:%M");
					std::cout << ",";
				rooms.push_back(row);
				std::cout << "\n";
			}
		}		
		
		return true;
	}
}

