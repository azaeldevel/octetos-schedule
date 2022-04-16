#ifndef OCTETOS_SCHEDULE_HH
#define OCTETOS_SCHEDULE_HH

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
		Enviroment* evprog;
		Gtk::Button* bt_apply;
		Gtk::Button* bt_stop;
		Gtk::Button* bt_close;
		//std::thread* run_evprog;
		Gtk::ProgressBar* pg_evprog;
		unsigned int count;
		bool stoped;
		Gtk::Label* lbIterations;
		Gtk::Expander* exp_logs;
		Gtk::Label* lb_leader;
		Gtk::Label* lb_mean;
		Gtk::Label* lb_sigma;
		unsigned int countP0;
		sigc::slot<bool()> slot;
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
		void on_bt_main_about_clicked();

	private:
		const Glib::RefPtr<Gtk::Builder> builder;
		Gtk::ToolButton* bt_main_open;
		Gtk::ToolButton* bt_main_analize;
		Gtk::ToolButton* bt_main_new;
		Gtk::ToolButton* bt_main_save;
		Gtk::ToolButton* bt_main_saveas;
		Gtk::ToolButton* bt_main_close;
		Gtk::ToolButton* bt_main_about;
		Gtk::Notebook* ntb_project;
		Gtk::Box* box_main;
		Analyzer* dlgAnalyzer;
		Enviroment* evprog;
		Project* project;
		bool project_saved;
		bool project_open;
		std::filesystem::path project_path;
		std::filesystem::path result_path;

	private:
		void append_config();
		void append_teachers();
		bool load_update_config(const std::filesystem::path&);//to fileproject
		bool read_project();

	private:
		using week_name = oct::ec::sche::Configuration::SchemaWeek;
		class ModelColumns : public Gtk::TreeModel::ColumnRecord
  		{
  		public:

    		ModelColumns();

    		Gtk::TreeModelColumn<week_name> id;
  		};
		struct PageConfig
		{
			Gtk::Fixed container;
			
			Gtk::Box box_config;
			Gtk::Box box_seconds;
			Gtk::Box box_week;
			Gtk::Box box_childs;
			Gtk::Box box_progenitors;
			Gtk::Box box_mutation_prob;
			Gtk::Box box_mutation_max;
			Gtk::Box box_directory;
			
			Gtk::Label lb_seconds;
			Gtk::Entry in_seconds;
			Gtk::Label lb_week;
			Gtk::ComboBox cmb_week;
  			Gtk::CellRendererText id_cell;
  			Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
			ModelColumns columns;
			Gtk::Label lb_childs;
			Gtk::Entry in_childs;
			Gtk::Label lb_progenitors;
			Gtk::Entry in_progenitors;
			Gtk::Label lb_mutation_prob;
			Gtk::Entry in_mutation_prob;
			Gtk::Label lb_mutation_max;
			Gtk::Entry in_mutation_max;
			Gtk::Label lb_directory;			
			Gtk::FileChooserButton bt_directory;
			std::filesystem::path directory;

			PageConfig();
			void show();
			void on_cell_id(const Gtk::TreeModel::const_iterator& iter);
			void on_combo_changed();
		};


		PageConfig* page_config;
	};

}
#endif
