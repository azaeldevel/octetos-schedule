#ifndef MUPOSYS_MAIN_HH
#define MUPOSYS_MAIN_HH

/**
 *
 *  This file is part of muposys.
 *  muposys is a Multi-Porpuse Software System GUI.
 *  Copyright (C) 2018  Azael Reyes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */


#include <gtkmm.h>
#include <thread>

#include "schedule.hh"
#include "Project.hh"

using namespace oct::ec::sche;

namespace sche
{

	class AboutDialog : public Gtk::AboutDialog
	{
	private:
		const Glib::RefPtr<Gtk::Builder>& builder;

	public:
		AboutDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	};

	class Analyzer : public Gtk::Dialog
	{
	public:
		Analyzer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade,Enviroment* const);
		~Analyzer();

	protected:
		void on_bt_apply_clicked();
		void on_bt_stop_clicked();
		void on_bt_close_clicked();

	private:
		//
		bool update_progress(int);

		//
		const Glib::RefPtr<Gtk::Builder>& builder;
		Enviroment* const evprog;
		Gtk::Button* bt_apply;
		Gtk::Button* bt_stop;
		Gtk::Button* bt_close;
		std::thread* run_evprog;
		Gtk::ProgressBar* pg_evprog;
		unsigned int count;
		bool stoped;
		//Gtk::Label* lb_prediction;		
	};

	class Main : public Gtk::Window
	{
	public:

		/**
		*
		**/
		Main(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
		~Main();
		
		const char* titleWindow()const;
		const char* systemName()const;

	protected:
		void on_bt_main_open_clicked();
		void on_bt_main_analize_clicked();
		void on_bt_main_new_clicked();
		void on_bt_main_save_clicked();
		void on_bt_main_saveas_clicked();
		void on_bt_main_close_clicked();

	private:
		const Glib::RefPtr<Gtk::Builder> builder;
		Gtk::ToolButton* bt_main_open;
		Gtk::ToolButton* bt_main_analize;
		Gtk::ToolButton* bt_main_new;
		Gtk::ToolButton* bt_main_save;
		Gtk::ToolButton* bt_main_saveas;
		Gtk::ToolButton* bt_main_close;
		Analyzer* dlgAnalyzer;
		Enviroment* evprog;
		Project* project;
		bool project_saved;
		std::filesystem::path project_path;
		std::filesystem::path result_path;
	};

}
#endif
