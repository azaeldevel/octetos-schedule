
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
#include "config.h"

namespace sche
{

AboutDialog::AboutDialog(BaseObjectType* o, const Glib::RefPtr<Gtk::Builder>& b) :  Gtk::AboutDialog(o), builder(b)
{

}



Main::Main(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : Gtk::Window(cobject), builder(refGlade),project(NULL)
{
	set_title(titleWindow());

	bt_main_open = 0;
	builder->get_widget("bt_main_open", bt_main_open);
	bt_main_open->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_open_clicked));

	bt_main_analize = 0;
	builder->get_widget("bt_main_analize", bt_main_analize);
	bt_main_analize->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_analize_clicked));

	bt_main_new = 0;
	builder->get_widget("bt_main_new", bt_main_new);
	bt_main_new->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_new_clicked));

	bt_main_save = 0;
	builder->get_widget("bt_main_save", bt_main_save);
	bt_main_save->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_save_clicked));

	bt_main_saveas = 0;
	builder->get_widget("bt_main_saveas", bt_main_saveas);
	bt_main_saveas->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_saveas_clicked));

	bt_main_close = 0;
	builder->get_widget("bt_main_close", bt_main_close);
	bt_main_close->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_close_clicked));

	bt_main_about = 0;
	builder->get_widget("bt_main_about", bt_main_about);
	bt_main_about->signal_clicked().connect(sigc::mem_fun(*this,&Main::on_bt_main_about_clicked));

	evprog = NULL;
	set_icon_name("/sche/schedule.ico");
	project_saved = true;
}
Main::~Main()
{	
	if(not evprog) delete evprog;
}
const char* Main::titleWindow()const
{
	return "sche";
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
		try
		{
			evprog = new Enviroment(result_path,project_path,result_path);
		}
		catch(const std::exception& e)
		{
			Gtk::MessageDialog dialog(*this, "Error ineperador",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
	  		dialog.set_secondary_text(e.what());
	  		dialog.run();
		}
		evprog->stopperMinSolutions(1);
		//std::cout << "Enviroment created\n";
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
	dlgAnalyzer = 0;
	if(evprog)
	{
		builder->get_widget_derived("dlgAnalyzer", dlgAnalyzer,evprog);
		int response = dlgAnalyzer->run();
	}
	else
	{
		Gtk::MessageDialog dialog(*this, "Informacion incompleta",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
  		dialog.set_secondary_text("Es necesario abrir el proyecto antes comenzar el analizis");
  		dialog.run();
	}
}
void Main::on_bt_main_new_clicked()
{
	if(project)
	{
		Gtk::MessageDialog dialog(*this, "Archivo abierto",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
	  	dialog.set_secondary_text("Hay un archivo abierto, cierre primero antes de continuar.");
	  	dialog.run();	
	  	return;
	}
	
	project = new Project();
	project->create();
	
	std::string msg = std::string(titleWindow()) + " - *";
	set_title(msg.c_str());
	project_saved = false;
}
void Main::on_bt_main_save_clicked()
{
	if(project_saved) return;
	if(not project)
	{
		Gtk::MessageDialog dialog(*this, "Archivo abierto",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
	  	dialog.set_secondary_text("No hay archivo abierto");
	  	dialog.run();	
	  	return;
	}
	
	//
	if(project_path.empty())
	{
		GtkWidget * dialog = gtk_file_chooser_dialog_new("Guardar",NULL,GTK_FILE_CHOOSER_ACTION_SAVE,"_Cancel",GTK_RESPONSE_CANCEL, "_Guardar", GTK_RESPONSE_ACCEPT, NULL);
		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
		{
			char* tmpfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
			gtk_widget_destroy (dialog);
			project_path = tmpfilename;
			g_free(tmpfilename);
			gtk_widget_destroy (dialog);
		}
		else
		{
			gtk_widget_destroy (dialog);
			return;
		}
	}
	if(std::filesystem::exists(project_path))
	{
		Gtk::MessageDialog dialog(*this, "Archivo existen",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_YES_NO);
	  	dialog.set_secondary_text("El archivo inidicado ya existe, Desea sobrescribir?");
	  	int  ret = dialog.run();
	  	if(ret == GTK_RESPONSE_YES)
	  	{
	  		std::filesystem::remove(project_path);
	  	}	
	  	else
	  	{
	  		return;
	  	}
	}
	
	//
	project->save(project_path);
	
	
	//
	std::string msg = std::string(titleWindow()) + " - " + project_path.filename().string(); 
	set_title(msg.c_str());
	project_saved = true;
}
void Main::on_bt_main_saveas_clicked()
{
	if(not project)
	{
		Gtk::MessageDialog dialog(*this, "Archivo abierto",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
	  	dialog.set_secondary_text("No hay archivo abierto");
	  	dialog.run();	
	  	return;
	}
	
	//
	GtkWidget * dialog = gtk_file_chooser_dialog_new("Guardar",NULL,GTK_FILE_CHOOSER_ACTION_SAVE,"_Cancel",GTK_RESPONSE_CANCEL, "_Guardar", GTK_RESPONSE_ACCEPT, NULL);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
	{
		char* tmpfilename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (dialog));
		gtk_widget_destroy (dialog);
		project_path = tmpfilename;
		g_free(tmpfilename);
		gtk_widget_destroy (dialog);
	}
	else
	{
		gtk_widget_destroy (dialog);
		return;
	}
	if(std::filesystem::exists(project_path))
	{
		Gtk::MessageDialog dialog(*this, "Archivo existen",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_YES_NO);
	  	dialog.set_secondary_text("El archivo inidicado ya existe, Desea sobrescribir?");
	  	int  ret = dialog.run();
	  	if(ret == GTK_RESPONSE_YES)
	  	{
	  		std::filesystem::remove(project_path);
	  	}	
	  	else
	  	{
	  		return;
	  	}
	}
		
	//
	project->save(project_path);	
	
	//
	std::string msg = std::string(titleWindow()) + " - " + project_path.filename().string(); 
	set_title(msg.c_str());
	project_saved = true;
}
void Main::on_bt_main_close_clicked()
{
	if(not project_saved)
	{
		Gtk::MessageDialog dialog(*this, "Archivo abierto",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_YES_NO);
	  	dialog.set_secondary_text("Hay un archivo sin guardar, Desea guardarlo antes de continuar?");
	  	int ret = dialog.run();
	  	if(ret == GTK_RESPONSE_YES) on_bt_main_save_clicked();
	}
	if(not project) return;
	
	delete project;
	project = NULL;
	set_title(titleWindow());
}
void Main::on_bt_main_about_clicked()
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/sche/schedule.ico", NULL);

  	GtkWidget *dialog = gtk_about_dialog_new();
  	//gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Schedule");
  	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), PACKAGE_VERSION); 
  	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Azael Reyes");
  	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Organizador de horarios Escolar");
  	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/azaeldevel/octetos-schedule.git");
  	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  	g_object_unref(pixbuf), pixbuf = NULL;
  	gtk_dialog_run(GTK_DIALOG (dialog));
  	gtk_widget_destroy(dialog);
}
}
