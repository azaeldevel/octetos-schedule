
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

 #include <sstream>


#include "Main.hh"

namespace sche
{

	Analyzer::Analyzer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& b,Enviroment* ep) : Gtk::Dialog(cobject), builder(b),evprog(ep),run_evprog(NULL)
	{
		bt_apply = 0;
		builder->get_widget("bt_Analyzer_Apply", bt_apply);
		bt_apply->signal_clicked().connect(sigc::mem_fun(*this,&Analyzer::on_bt_apply_clicked));

		bt_stop = 0;
		builder->get_widget("bt_Analyzer_Stop", bt_stop);
		bt_stop->signal_clicked().connect(sigc::mem_fun(*this,&Analyzer::on_bt_stop_clicked));
		bt_stop->set_sensitive(false);

		bt_close = 0;
		builder->get_widget("bt_Analyzer_Close", bt_close);
		bt_close->signal_clicked().connect(sigc::mem_fun(*this,&Analyzer::on_bt_close_clicked));

		exp_logs = 0;
		builder->get_widget("exp_logs", exp_logs);
		lbIterations = 0;
		builder->get_widget("lbIterations", lbIterations);
		lb_leader = 0;
		builder->get_widget("lb_leader", lb_leader);
		lb_mean = 0;
		builder->get_widget("lb_mean", lb_mean);
		lb_sigma = 0;
		builder->get_widget("lb_sigma", lb_sigma);

		pg_evprog = 0;
		builder->get_widget("pg_evprog", pg_evprog);

		count = 0;
		stoped = false;
		countP0 = 0;
	}

	Analyzer::~Analyzer()
	{
		if(not run_evprog) delete run_evprog;
		//std::cout << "Analyzer::~Analyzer\n";
	}



	void th_run(void* obj)
	{
	    try
	    {
	    	//std::cout << "Running..\n";
            ((Enviroment*)obj)->run();
        }
		catch(const std::exception& e)
		{
            std::filesystem::path dir;
            if(not ((Enviroment*)obj)->getLogDirectory().empty()) dir = ((Enviroment*)obj)->getLogDirectory();
            else if(not ((Enviroment*)obj)->getLogDirectorySolutions().empty()) dir = ((Enviroment*)obj)->getLogDirectorySolutions();
            else if(not ((Enviroment*)obj)->getLogDirectoryHistory().empty()) dir = ((Enviroment*)obj)->getLogDirectoryHistory();

            if(not dir.empty())
            {
            	std::ofstream errofile;
            	errofile.open(dir/"errors");
            	errofile << e.what();
            	errofile.flush();
            	errofile.close();
            }
		}
		catch(...)
		{
			std::exception_ptr p = std::current_exception();

		    std::filesystem::path dir;
            if(not ((Enviroment*)obj)->getLogDirectory().empty()) dir = ((Enviroment*)obj)->getLogDirectory();
            else if(not ((Enviroment*)obj)->getLogDirectorySolutions().empty()) dir = ((Enviroment*)obj)->getLogDirectorySolutions();
            else if(not ((Enviroment*)obj)->getLogDirectoryHistory().empty()) dir = ((Enviroment*)obj)->getLogDirectoryHistory();

            if(not dir.empty())
            {
            	std::ofstream errofile;
            	errofile.open(dir/"errors");
            	errofile << "Excepcion desconocida.. ";
            	errofile << (p ? p.__cxa_exception_type()->name() : "null") << std::endl;;
            	errofile.flush();
            	errofile.close();
            }
		}
    }
	void Analyzer::on_bt_apply_clicked()
	{
		slot = sigc::bind(sigc::mem_fun(*this,&Analyzer::update_progress),1);
		Glib::signal_timeout().connect(slot, 1000);

		//run_evprog = new std::thread(th_run,evprog);
		bt_apply->set_sensitive(false);
		bt_close->set_sensitive(false);
		bt_stop->set_sensitive(true);
		stoped = false;
	}

	void Analyzer::on_bt_stop_clicked()
	{
		evprog->stop();
		bt_apply->set_sensitive(true);
		bt_close->set_sensitive(true);
		stoped = true;
	}

	void Analyzer::on_bt_close_clicked()
	{
		close();
	}

	bool Analyzer::update_progress(int )
	{
		if(evprog->get_population_size() == 0)
		{
			/*on_bt_stop_clicked();
			Gtk::MessageDialog dialog(*this, "Error ineperador",false, Gtk::MESSAGE_ERROR,Gtk::BUTTONS_OK);
	  		dialog.set_secondary_text("La poblacion actual es 0");
	  		dialog.run();*/
			countP0++;
			std::cout << "countP0 : " << countP0 << "\n";
	  		return true;
		}
		if(evprog->isRunning())
		{
			double progress,percen;
			std::string str_display;
			//std::string str_predict;

			progress = evprog->getProgress();
			progress = round(double(100000000) * progress);
			progress /= double(100000000);
			percen = progress * double(100);
			str_display.clear();
			str_display = std::to_string(percen) + "%";
			if(count < 6)
            {
                count++;
                switch(count)
                {
                case 1:
                    str_display += ".    ";
                    break;
                case 2:
                    str_display += "..   ";
                    break;
                case 3:
                    str_display += "...  ";
                    break;
                case 4:
                    str_display += ".... ";
                    break;
                case 5:
                    str_display += ".....";
                    count = 0;
                    break;
                }
            }
			pg_evprog->set_fraction(progress);
			pg_evprog->set_text(str_display);
			if(exp_logs->get_expanded())
			{
				lbIterations->set_text(std::to_string(evprog->getIterationActual()));
				std::ostringstream ssnumber;
				std::string snumber;
				ssnumber << std::fixed;
				ssnumber << std::setprecision(15);
				ssnumber << evprog->get_single(0)->getFitness();
				snumber = ssnumber.str();
				lb_leader->set_text(snumber);
                ssnumber.str("");
				ssnumber.clear();
                ssnumber << evprog->getMean();
                snumber = ssnumber.str();
				lb_mean->set_text(snumber);
				ssnumber.str("");
				ssnumber.clear();
                ssnumber << evprog->getSigma();
                snumber = ssnumber.str();
				lb_sigma->set_text(snumber);
			}
		}
		else if(evprog->getSolutions().size() > 0)
		{
			Gtk::MessageDialog dialog(*this, "Operacion completada");
  			dialog.set_secondary_text("Se encontro la(s) soluciones requeridadas");
  			dialog.run();
		}
		else if(stoped)
		{
			pg_evprog->set_text("Detenido...");
		}
		else
		{
			pg_evprog->set_fraction(0);
			pg_evprog->set_text("Analisis Fase 1..");
		}

		return true;
	}




}
