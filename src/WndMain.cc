
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

#if defined(__GNUC__) && defined(__linux__)
    #include "config.h"
#elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))

#else
    #error "Pltaforma desconocida"
#endif

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

	box_main = 0;
	builder->get_widget("box_main", box_main);
	
	evprog = NULL;
	set_icon_name("/sche/schedule.ico");
	project_saved = true;
	project_open = false;
	
	page_config = NULL;
	
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
	project_open = true;
	
	ntb_project = new Gtk::Notebook();
	box_main->pack_start(*ntb_project);
	ntb_project->show();
	append_config();
	append_teachers();
}
Main::PageConfig::PageConfig()
{
	//boxes
	box_config.set_orientation(Gtk::ORIENTATION_VERTICAL);
	box_childs.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_config.pack_start(box_childs);
	box_progenitors.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_config.pack_start(box_progenitors);
	box_mutation_prob.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_config.pack_start(box_mutation_prob);
	box_mutation_max.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_config.pack_start(box_mutation_max);
	box_directory.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
	box_config.pack_start(box_directory);
	container.add(box_config);
	
	//controls
	box_childs.pack_start(lb_childs);
	box_childs.pack_start(in_childs);
	lb_childs.set_text("Max. cant. de variable(Hijos) : ");
	lb_childs.set_halign(Gtk::ALIGN_START);
	in_childs.set_halign(Gtk::ALIGN_START);
	in_childs.set_input_purpose(Gtk::INPUT_PURPOSE_NUMBER);
	
	box_progenitors.pack_start(lb_progenitors);
	box_progenitors.pack_start(in_progenitors);
	lb_progenitors.set_text("Max. cant. variables conservadas(Progenitores) : ");
	lb_progenitors.set_halign(Gtk::ALIGN_START);
	in_progenitors.set_halign(Gtk::ALIGN_START);
	in_progenitors.set_input_purpose(Gtk::INPUT_PURPOSE_NUMBER);
	
	box_mutation_prob.pack_start(lb_mutation_prob);
	box_mutation_prob.pack_start(in_mutation_prob);
	lb_mutation_prob.set_text("Taza de cambio(Mutaciones) : ");
	lb_mutation_prob.set_halign(Gtk::ALIGN_START);
	in_mutation_prob.set_halign(Gtk::ALIGN_START);
	in_mutation_prob.set_input_purpose(Gtk::INPUT_PURPOSE_NUMBER);
		
	box_mutation_max.pack_start(lb_mutation_max);
	box_mutation_max.pack_start(in_mutation_max);
	lb_mutation_max.set_text("Max. cant. cambios(Max. Mutacion) : ");
	lb_mutation_max.set_halign(Gtk::ALIGN_START);
	in_mutation_max.set_halign(Gtk::ALIGN_START);
	in_mutation_max.set_input_purpose(Gtk::INPUT_PURPOSE_NUMBER);
	
	box_directory.pack_start(lb_directory);
	box_directory.pack_start(bt_directory);
	lb_directory.set_text("Directorio de Resultados : ");
	lb_directory.set_halign(Gtk::ALIGN_START);
	bt_directory.set_halign(Gtk::ALIGN_START);
}
void Main::PageConfig::show()
{
	container.show();
	
	box_config.show();
	box_childs.show();
	box_progenitors.show();
	box_mutation_prob.show();
	box_mutation_max.show();
	box_directory.show();
	
	lb_childs.show();
	in_childs.show();
	lb_progenitors.show();
	in_progenitors.show();
	lb_mutation_prob.show();
	in_mutation_prob.show();
	lb_mutation_max.show();
	in_mutation_max.show();
	lb_directory.show();
	bt_directory.show();
	
}
void Main::append_config()
{
	page_config = new PageConfig;
	page_config->show();
	ntb_project->append_page(page_config->container, "Configuracion");
}
void Main::append_teachers()
{
	Gtk::Label* lb_teachers = new Gtk::Label("Teachers");
  	ntb_project->append_page(*lb_teachers, "Maestros");
  	lb_teachers->show();
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
		Gtk::MessageDialog dialog(*this, "Archivo abierto",false,Gtk::MESSAGE_ERROR,Gtk::BUTTONS_YES_NO);
	  	dialog.set_secondary_text("Hay un archivo sin guardar, Desea guardarlo antes de continuar?");
	  	int ret = dialog.run();
	  	if(ret == GTK_RESPONSE_YES) on_bt_main_save_clicked();
	}
	if(not project) return;

	delete project;
	project = NULL;
	delete evprog;
	evprog = NULL;
	set_title(titleWindow());
	project_open = false;
}
void Main::on_bt_main_about_clicked()
{
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource("/sche/schedule.ico", NULL);
  	GtkWidget *dialog = gtk_about_dialog_new();
  	//gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Schedule");
#if defined(__linux__)
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), PACKAGE_VERSION);
#elif (defined(_WIN32) || defined(_WIN64))
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "alpha 31.0");
#else
    #error "Plataforma desconocida"
#endif
  	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"(c) Azael Reyes");
  	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Organizador de horarios Escolar");
  	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog), "https://github.com/azaeldevel/octetos-schedule.git");
  	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
  	g_object_unref(pixbuf), pixbuf = NULL;
  	gtk_dialog_run(GTK_DIALOG (dialog));
  	gtk_widget_destroy(dialog);
}



bool Main::load_update_config()
{
	project->ep_config->set_max_population(std::stoi(page_config->in_childs.get_text()));
	
	project->ep_config->set_max_progenitor(std::stoi(page_config->in_progenitors.get_text()));
	
	project->ep_config->set_mutable_prob(std::stoi(page_config->in_mutation_prob.get_text()));
	
	project->ep_config->set_max_mutation(std::stoi(page_config->in_mutation_max.get_text()));
		
	project->ep_config->set_out_directory(page_config->directory);
	
	return true;
}

}
