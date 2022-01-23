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




#include "Main.hh"
#include <iostream>



namespace sche
{

AboutDialog::AboutDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : builder(refGlade)
{
	
}






Main::Main(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder(refGlade)
{
	set_title(titleWindow());
	
	builder->get_widget("bt_main_open", bt_main_open);
	bt_main_open->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_open_clicked));

}
const char* Main::titleWindow()const
{
	return "Schedule - Octetos";
}
const char* Main::systemName()const
{
	return "Schedule";
}


void Main::on_bt_main_open_clicked()
{
	char* tmpfilename;
	GtkWidget *dialog;
	
	dialog = gtk_file_chooser_dialog_new("Seleccione directorio de Proyecto",NULL,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,"_Cancel",GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char* tmpfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		std::cout << "Directory : " << tmpfilename << "\n";
		gtk_widget_destroy (dialog);
		g_free (tmpfilename);
	}  
		
	dialog = gtk_file_chooser_dialog_new("Seleccione directorio de Resultado",NULL,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,"_Cancel",GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		tmpfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		std::cout << "Directory : " << tmpfilename << "\n";
		gtk_widget_destroy (dialog);
		g_free (tmpfilename);
	}
}


}