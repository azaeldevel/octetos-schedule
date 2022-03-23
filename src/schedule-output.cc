
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


#include "schedule-data.hh"

namespace oct::ec::sche::output
{




	Student::Student()
	{
		group = NULL;
	}
	Student::Student(const ClassRoom& cr)
	{
		if(cr.size() == 0) throw core::Exception("Hoario vacio",__FILE__,__LINE__);
		
		resize(cr.size());
		group = cr[0].group;
		for(unsigned int i = 0; i < cr.size(); i++)
		{
			if(group != cr[i].group) throw core::Exception("El valor grupo no coincide con todos los elemento",__FILE__,__LINE__);
			at(i).room = cr[i].room;
			at(i).subject = cr[i].subject;
			at(i).teacher = cr[i].teacher;
			at(i).week = cr[i].week;
		}
	}
	Student& Student::operator =(const ClassRoom& cr)
	{
		if(cr.size() == 0) throw core::Exception("Hoario vacio",__FILE__,__LINE__);
		
		resize(cr.size());
		group = cr[0].group;
		for(unsigned int i = 0; i < cr.size(); i++)
		{
			if(group != cr[i].group) throw core::Exception("El valor grupo no coincide con todos los elemento",__FILE__,__LINE__);
			at(i).room = cr[i].room;
			at(i).subject = cr[i].subject;
			at(i).teacher = cr[i].teacher;
			at(i).week = cr[i].week;
		}
		
		return *this;
	}
	void Student::print_csv(std::ostream& out,const Configuration& data) const
	{
		if(size() == 0) throw core::Exception("Hoario vacio",__FILE__,__LINE__);
		
		out << group->room->get_name() << "\n";
		for(unsigned int i = 0; i < size(); i++)
		{
			out << at(i).room->get_name() << "\n";
			out << at(i).subject->get_name() << ",";
			out << at(i).teacher->get_name() << ",";
			at(i).week.print_intevals_csv(out,data);
		}
	}
	
	
	
	
	
	
	
	
	
	
	Students::Students(const sche::Schedule& s)
	{
		resize(s.size());
		for(unsigned int i = 0; i < s.size(); i++)
		{
			at(i) = s[i];
		}
	}
	
	void Students::print_csv(std::ostream& out,const Configuration& data) const
	{
		for(unsigned int i = 0; i < size(); i++)
		{
			at(i).print_csv(out,data);
		}
	}
}