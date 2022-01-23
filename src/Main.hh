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


namespace sche
{

	class AboutDialog : public Gtk::AboutDialog
	{
	private:
		const Glib::RefPtr<Gtk::Builder>& builder;

	public:
		AboutDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	};

	class Main : public Gtk::Window
	{
	public:

		/**
		*
		**/
		Main(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);		
		bool on_button_press(GdkEventButton* event);		
		const char* titleWindow()const;
		const char* systemName()const;
		//carga de gui no gestionadna por glade
		
	protected:
		void on_bt_main_open_clicked();
	private:
		const Glib::RefPtr<Gtk::Builder> builder;
		Gtk::ToolButton* bt_main_open;
	};

}
#endif 
