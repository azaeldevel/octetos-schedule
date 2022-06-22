
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
        in_seconds = 0;
        builder->get_widget("in_config_seconds", in_seconds);

        in_population = 0;
        builder->get_widget("in_config_population", in_population);

        in_progenitor = 0;
        builder->get_widget("in_config_progenitor", in_progenitor);

        in_mutation_prob = 0;
        builder->get_widget("in_config_mutation_prob", in_mutation_prob);

        in_max_mutations = 0;
        builder->get_widget("in_config_max_mutations", in_max_mutations);

        in_seconds->set_text(std::to_string(p->get_data().config.get_seconds_per_hour()));
        in_population->set_text(std::to_string(p->get_data().config.get_max_population()));
        in_progenitor->set_text(std::to_string(p->get_data().config.get_max_progenitor()));
        in_mutation_prob->set_text(std::to_string(p->get_data().config.get_mutable_prob()));
        in_max_mutations->set_text(std::to_string(p->get_data().config.get_max_mutation()));

	}

	Configuration::~Configuration()
	{
	}

}
