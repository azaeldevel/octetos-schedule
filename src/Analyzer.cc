
#include "Main.hh"

namespace sche
{

	Analyzer::Analyzer(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& b,Enviroment* ep) : Gtk::Dialog(cobject), builder(b),evprog(ep),run_evprog(NULL)
	{
		bt_ok = 0;
		builder->get_widget("bt_Analyzer_OK", bt_ok);
		bt_ok->signal_clicked().connect(sigc::mem_fun(*this,&Analyzer::on_bt_ok_clicked));
	}
	/*bool update_progress()
	{

		return true;
	}*/
	void th_run(void* obj)
	{
        ((Enviroment*)obj)->run();
    }
	void Analyzer::on_bt_ok_clicked()
	{
		/*auto update_progress = [this]() -> bool
		{
			std::cout << "Progress : " << evprog->getProgress() << "\n";
			pb_main_pregress->set_fraction(evprog->getProgress());
			return true;
		};*/
		//sigc::slot<bool> tslot = sigc::ptr_fun(&update_progress);
		sigc::slot<bool()> slot = sigc::bind(sigc::mem_fun(*this,&Analyzer::update_progress),1);
		Glib::signal_timeout().connect(slot, 1000);
		run_evprog = new std::thread(th_run,&evprog);
		bt_ok->set_sensitive(false);
	}

	bool Analyzer::update_progress(int )
	{
		std::cout << "Progress : " << evprog->getProgress() << "\n";
		pb_main_pregress->set_fraction(evprog->getProgress());

		return true;
	}
}
