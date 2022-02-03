
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
		std::cout << "Progress : " << evprog->getProgress() << "\n";
		pg_evprog->set_fraction(evprog->getProgress());

		return true;
	}
}
