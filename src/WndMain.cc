
/**
 *
 *  This file is part of schedule.
 *  Copyright (C) 2018  Azael Reyes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 */

#include <iostream>


#include "Main.hh"


namespace sche
{

AboutDialog::AboutDialog(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :  Gtk::AboutDialog(cobject), builder(refGlade)
{
	
}

Analyzer::Analyzer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Dialog(cobject), builder(refGlade)
{
	
}


Main::Main(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder(refGlade)
{
	set_title(titleWindow());
	
	builder->get_widget("bt_main_open", bt_main_open);
	bt_main_open->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_open_clicked));
	
	builder->get_widget("bt_main_analize", bt_main_analize);
	bt_main_analize->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_analize_clicked));
	
	evprog = NULL;
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
		//std::cout << "Directory : " << tmpfilename << "\n";
		gtk_widget_destroy (dialog);
		project_path = tmpfilename;
		g_free (tmpfilename);
	}  
	else
	{
		gtk_widget_destroy (dialog);
	}
		
	dialog = gtk_file_chooser_dialog_new("Seleccione directorio de Resultado",NULL,GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,"_Cancel",GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		tmpfilename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		//std::cout << "Directory : " << tmpfilename << "\n";
		gtk_widget_destroy (dialog);
		result_path = tmpfilename;
		g_free (tmpfilename);
	} 
	else
	{
		gtk_widget_destroy (dialog);
	}
	
	if(not project_path.empty() and not result_path.empty())
	{
		evprog = new Enviroment(project_path,project_path,project_path);
	}
	else
	{
		Gtk::MessageDialog dialog(*this, "Informacion incompleta",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
  		dialog.set_secondary_text("Es necesario que indique ambas rutas previamente para poder iniciar el proceso de Analisis.");
  		dialog.run();
	}
}
void Main::on_bt_main_analize_clicked()
{
	dlg_analyzer = 0;
	builder->get_widget_derived("dlg_analyzer", dlg_analyzer);
	int response = dlg_analyzer->run();
}

}
