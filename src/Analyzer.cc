
#include "Main.hh"

namespace sche
{

	Analyzer::Analyzer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& b,Enviroment* const ep) : Gtk::Dialog(cobject), builder(b),evprog(ep),run_evprog(NULL)
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

		pg_evprog = 0;
		builder->get_widget("pg_evprog", pg_evprog);

		count = 0;
		stoped = false;

		run_evprog = NULL;
	}

	Analyzer::~Analyzer()
	{
		if(not run_evprog) delete run_evprog;
	}



	void th_run(void* obj)
	{
	    try
	    {
            ((Enviroment*)obj)->run();
        }
		catch(const std::exception& e)
		{
            std::cerr << e.what() << std::endl;
		}
    }
	void Analyzer::on_bt_apply_clicked()
	{
		sigc::slot<bool()> slot = sigc::bind(sigc::mem_fun(*this,&Analyzer::update_progress),1);
		Glib::signal_timeout().connect(slot, 1000);
		run_evprog = new std::thread(th_run,evprog);
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
			//str_predict = std::to_string(evprog->getIterationActual()) + "/" + std::to_string(evprog->getPredictFinally());
			//lb_prediction->set_text(str_predict.c_str());
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
