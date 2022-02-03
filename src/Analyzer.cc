
#include "Main.hh"

namespace sche
{

	Analyzer::Analyzer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& b,Enviroment* ep) : Gtk::Dialog(cobject), builder(b),evprog(ep),run_evprog(NULL)
	{
		bt_ok = 0;
		builder->get_widget("bt_Analyzer_OK", bt_ok);
		bt_ok->signal_clicked().connect(sigc::mem_fun(*this,&Analyzer::on_bt_ok_clicked));
		pg_evprog = 0;
		builder->get_widget("pg_evprog", pg_evprog);
	}
	/*
	bool update_progress()
	{

		return true;
	}
	*/
	void th_run(void* obj)
	{
		//std::cout << "Step th_run : 1\n";
        ((Enviroment*)obj)->run();
        //std::cout << "Step th_run : 2\n";
    }
	void Analyzer::on_bt_ok_clicked()
	{
		/*
		auto update_progress = [this]() -> bool
		{
			std::cout << "Progress : " << evprog->getProgress() << "\n";
			pb_main_pregress->set_fraction(evprog->getProgress());
			return true;
		};
		*/
		//sigc::slot<bool> tslot = sigc::ptr_fun(&update_progress);
		//std::cout << "Step 1\n";
		sigc::slot<bool()> slot = sigc::bind(sigc::mem_fun(*this,&Analyzer::update_progress),1);
		//std::cout << "Step 2\n";
		Glib::signal_timeout().connect(slot, 1000);
		//std::cout << "Step 3\n";
		run_evprog = new std::thread(th_run,evprog);
		//std::cout << "Step 4\n";
		//evprog->run();
		bt_ok->set_sensitive(false);
	}

	bool Analyzer::update_progress(int )
	{
		if(evprog->isRunning())
		{
			double progress,percen;
			std::string str_display;

			progress = evprog->getProgress();
			progress = round(double(1000000) * progress);
			progress /= double(1000000);
			percen = progress * double(100);
			str_display = std::to_string(percen) + "%";
			pg_evprog->set_fraction(progress);
			pg_evprog->set_text(str_display);
		}
		else
		{
			pg_evprog->set_fraction(0);
			pg_evprog->set_text("Analisis Fase 1..");
		}

		return true;
	}
}
