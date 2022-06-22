
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


#include "Main.hh"

namespace sche
{

	Configuration::Configuration(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& b, const Project* p) : Gtk::Dialog(cobject), builder(b),project(p)
	{
        in_population = 0;
        builder->get_widget("in_population", in_population);


        in_population->set_text(std::to_string(p->get_data().config.get_max_population()));
	}

	Configuration::~Configuration()
	{
	}

}
