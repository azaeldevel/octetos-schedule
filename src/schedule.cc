
#include <iomanip>
#include <typeinfo>
#include <typeindex>

#include "schedule.hh"


namespace oct::core
{

}


namespace oct::ec::sche
{

	unsigned int Single::jump_saver = 10;

	Single::Single(Enviroment& env,unsigned int c) : ec::Single(env,c)
	{
	}
	/*Single::Single(ID id,Enviroment& env, const Schedule& s) : ec::Single(id,env), Schedule(s)
	{
		random_algorit();
	}*/
	Single::Single(Enviroment& env) : ec::Single(env)
	{
	}

	Single::algorit Single::random_algorit()
	{
		std::uniform_int_distribution<> distrib(1,4);
		switch(distrib(gen))
		{
			case 1:
				return &Schedule::juncting_mesh_lessons;
			case 2:
				return &Schedule::juncting_mesh_classroom;
			case 3:
				return &Schedule::juncting_half;
			case 4:
				return &Schedule::juncting_choose_one_lesson;
			case 5:
				return &Schedule::juncting_choose_random_lesson;
			default:
				throw core::Exception("Funcion de apareo desconocida",__FILE__,__LINE__);
		}

		return NULL;
	}
	void Single::save(Save& fn)
	{
		(*fn.out) << getID();
		(*fn.out) << ",";
		(*fn.out) << getFitness();
		(*fn.out) << "\n";

		unsigned int mod = env->getIterationActual() % jump_saver;
		std::type_index ti(typeid(fn));
		if(mod == 0 or ti == std::type_index(typeid(SaveSolutions)) or env->getIterationActual() == 1 or fn.new_leader)
		{
			const Configuration& config = ((Enviroment*)env)->get_data().config;

			std::filesystem::path dir = config.get_out_directory() / std::to_string(env->getIterationActual()) / std::to_string(getID());

			std::filesystem::create_directory(dir);
			save_csv(config,dir);
			if(env->getIterationActual() < 100) jump_saver = 10;
			else if(env->getIterationActual() < 1000) jump_saver = 100;
			else if(env->getIterationActual() < 10000) jump_saver = 1000;
		}
	}
	void Single::juncting(std::list<oct::ec::Single*>& chils,const oct::ec::Single* single)
	{
		std::bernoulli_distribution distrib(0.5);
		for(unsigned int i = 0; i < getChilds(); i++)
		{
			Single* newsingle = new Single((Enviroment&)*env,getChilds());
			newsingle->resize(size());
			(newsingle->*(random_algorit()))(*this,*(const Single*)single);//llama a un algoritmo de mezclado al azar
			chils.push_back(newsingle);
		}
	}
	/*void Single::juncting_mesh_lessons(const Single& s1,const Single& s2)
	{
		Schedule::juncting_mesh_lessons(s1,s2);
	}
	void Single::juncting_mesh_classroom(const Single& s1,const Single& s2)
	{
		Schedule::juncting_mesh_classroom(s1,s2);
	}
	void Single::juncting_half(const Single& s1,const Single& s2)
	{
		Schedule::juncting_half(s1,s2);
	}
	void Single::juncting_choose_one_lesson(const Single& s1,const Single& s2)
	{
		Schedule::juncting_choose_one_lesson(s1,s2);
	}
	void Single::juncting_choose_random_lesson(const Single& s1,const Single& s2)
	{
		Schedule::juncting_choose_random_lesson(s1,s2);
	}*/
	void Single::print(std::ostream&) const
	{
	}
	void Single::mutate()
	{
		if(size() == 0) throw core::Exception("Hoario vacio",__FILE__,__LINE__);

		std::uniform_int_distribution<int> distrib(0, size() - 1);
		at(distrib(gen)).mutate();
	}






Enviroment::Enviroment()
{
}
Enviroment::Enviroment(const std::filesystem::path& log,const std::filesystem::path& in,const std::filesystem::path& out) : data(in,out)
{
	init(log,in,out);
}
Enviroment::~Enviroment()
{
}
void Enviroment::init(const std::filesystem::path& log,const std::filesystem::path& in,const std::filesystem::path& out)
{
	logDirectoryHistory = log;
	logDirectorySolutions = log;
	//logDirectory = log;

	//if(not shell.exists(in_dir)) shell.mkdir(in_dir,true);
	if(not std::filesystem::exists(out)) std::filesystem::create_directory(out);

    data.load(in);

	init(in);
}

void Enviroment::init(const std::filesystem::path& in_dir)
{
	mutableProb = 0.05;

	if(not in_dir.empty())
	{
		input_directory = in_dir;
		initPopulation = data.config.get_trys() / 10;
		maxProgenitor = initPopulation;
		maxPopulation = data.config.get_trys();
	}
	else
	{
		throw oct::core::Exception("Deve asignar el archivo de proyecto.",__FILE__,__LINE__);
	}

	CRITERION = 3;
	//SCHEDULE_ERROR = 0;
	schedule_overlap_max = data.groups.get_list().size() * data.groups.get_max_lessons() * Single::WEEK_HOURS/2;
	schedule_overlap_max2 = std::pow(schedule_overlap_max,2);
	schedule_cover_max = data.groups.get_list().size() * data.groups.get_max_lessons() * (Single::WEEK_HOURS - 7);
	schedule_cover_max2 = std::pow(schedule_cover_max,2);
	SCHEDULE_EMPTY_MAX = data.groups.get_list().size() * data.groups.get_max_lessons() * Single::WEEK_HOURS;
	schedule_sigma_hours_limit = data.config.get_hours_sigma();
	//Se asumen 4 horas en 0 y 3 en 24
	real mean = real(24 * 3) / real(7);
	real sigma = ((3.0 * std::pow(24.0 - mean,2.0)) + (4.0 * std::pow(mean,2.0)));
	sigma /= 7.0;
	schedule_sigma_hours_max = sigma;
	schedule_sigma_hours_max2 = std::pow(schedule_sigma_hours_max,2.0);

	/*
	for(unsigned int i = 0; i < CRITERION; i++)
	{
		SCHEDULE_ERROR += 1.0/real(CRITERION);
	}
	*/
	//SCHEDULE_ERROR = 1.0 - SCHEDULE_ERROR;
	//PORTION = 1.0/real(CRITERION);
	//schedule_max_hours = std::min((unsigned int)data.groups.get_list().size() * data.groups.get_max_lessons() * (Single::WEEK_HOURS/2), Single::WEEK_HOURS2) ;
	//GAMMA = 1.0/real(SCHEDULE_MAX_HOURS * CRITERION);
	//epsilon = 1.0 / real(std::pow(data.groups.get_list().size() * data.groups.get_max_lessons() * Single::WEEK_HOURS,2) * CRITERION);
	echoPrecision = 20;
}

void Enviroment::initial()
{
	if(echolevel > 0 and echoF != NULL) echoF("Poblando ambiente..");
	Schedules inits;
	inits.resize(initPopulation);

	if(initPopulation < data.groups.get_list().size() * 2) throw core::Exception("El tamano de la poblacion inicial es muy bajo",__FILE__,__LINE__);
	if((initPopulation % data.groups.get_list().size()) != 0 and (initPopulation / data.groups.get_list().size() ) < 2 ) throw core::Exception("La poblacion inicial deve ser multiplos de la cantida de grupos.",__FILE__,__LINE__);

	if(echolevel > 0 and echoF != NULL)
	{
		echoF(".");
		//fout->flush();
	}
	for(unsigned int i = 0; i < initPopulation; i++)
	{
		if(echolevel > 0 and echoF != NULL)
		{
		echoF(".");
		//fout->flush();
		}
		Single* sche = new Single(*this);
		sche->resize(data.groups.get_list().size());
		Groups::const_iterator itGroup = data.groups.get_list().begin();
		for(ClassRoom& lessons : *sche)
		{
			if(echolevel > 0 and echoF != NULL)
			{
				echoF(".");
				//fout->flush();
			}
			lessons.resize((*itGroup).size());
			std::vector<const Subject*>::const_iterator it_subject = (*itGroup).begin();
			unsigned int subject = 0;
			//std::cout << (&*itGroup)->room->get_name() << " : ";
			for(const Subject* subjectGroup : *itGroup)
			{
				if(echolevel > 0 and echoF != NULL)
				{
					echoF(".");
					//fout->flush();
				}
				//std::cout << "Enviroment::initial step : 1 \n";
				lessons[subject].group = &*itGroup;
				lessons[subject].room = (&*itGroup)->room;
				lessons[subject].subject = subjectGroup;
				std::list<const Teachers_Subjects::Row*> rows;
				data.teachers_subjects.searchSubjects(subjectGroup->get_name(),rows);
				//std::cout << lessons[subject].subject->get_name();
				std::list<const Teachers_Subjects::Row*>::const_iterator itr = random(rows);
				const Teachers_Subjects::Row* r = (itr != rows.end()) ? *itr : NULL;
				if(r)
				{
					lessons[subject].teacher = r->teacher;
					//std::cout << "(" << r->teacher->get_name() << "),";
				}
				else
				{
					std::string msg = "No se encontro maestro asociado para '";
					msg += subjectGroup->get_name() + "'";
					throw core::Exception(msg,__FILE__,__LINE__);
				}
				lessons[subject].data =&data;

				//std::cout << "ID : " << sche->getID() << "\n";
				WeekHours week;
				WeekOptions week_opt;
				//std::cout << "Step 1\n";
				//std::cout << "lessons[subject].room->get_week() count : " << lessons[subject].room->get_week().count_hours() << "\n";
				//std::cout << "lessons[subject].teacher->get_week() count : " << lessons[subject].teacher->get_week().count_hours() << "\n";
				week.inters(lessons[subject].room->get_week(),lessons[subject].teacher->get_week());

				/*std::cout << "Teacher : ";
				lessons[subject].teacher->get_week().print(std::cout);
				std::cout << "\n";
				std::cout << "Room : ";
				lessons[subject].room->get_week().print(std::cout);
				std::cout << "\n";
				std::cout << "Inteseccion : ";
				week.print(std::cout);
				std::cout << "\n";*/

				//std::cout << "Step 2\n";
				//check_codes code = week.check();
				week.sort(data.config);
				week.combns(*lessons[subject].subject,week_opt);
				//std::cout << "Step 3\n";

				//std::cout << "Enviroment::initial step : 2\n";
				//es un algoritmo que creara los horarios lo mas correctos posibles
				select_times(lessons[subject],week);
				if(lessons[subject].week.size() != WeekHours::WEEK_SIZE ) throw core::Exception("La semana no puede tener mas de 7 dias",__FILE__,__LINE__);
				//std::cout << "Enviroment::initial step : 3\n";
				//week_opt.random(lessons[subject].week);
				lessons[subject].week.sort(data.config);
				//std::cout << "ID : " << sche->getID() << ", week opt " << week_opt.count() << "\n";
				//std::cout << "ID : " << sche->getID() << ", " << lessons[subject].week.count_hours() << "\n";
				/*std::cout << "Result : ";
				lessons[subject].week.print(std::cout);
				std::cout << "\n";*/
				subject++;
				it_subject++;
				//std::cout << "Enviroment::initial step : 5 \n";
			}
			//std::cout << "\n";
			itGroup++;
		}
		sche->indexing();
		push_back(sche);
	}

	/*for(auto const& [key,value] : data.get_list_hbrs())
	{
		std::cout << "(" << key.room->get_name() << "," << key.subject->get_name() << "-->" << value.disp_hours << "\n";
	}*/

	if(echolevel > 0 and echoF != NULL)
	{
		echoF("\n--------------------------------------------------------------\n");
		//fout->flush();
	}
}
const Data& Enviroment::get_data()const
{
	return data;
}

unsigned int Enviroment::get_criterion() const
{
	return CRITERION;
}
unsigned int Enviroment::get_overlap_max() const
{
	return schedule_overlap_max;
}
unsigned long Enviroment::get_overlap_max2() const
{
	return schedule_overlap_max2;
}
unsigned int Enviroment::get_cover_max() const
{
	return schedule_cover_max;
}
unsigned long Enviroment::get_cover_max2() const
{
	return schedule_cover_max2;
}
unsigned int Enviroment::get_empty_max() const
{
	return SCHEDULE_EMPTY_MAX;
}
unsigned int Enviroment::get_sigma_hours_max() const
{
	return schedule_sigma_hours_max;
}
unsigned int Enviroment::get_sigma_hours_max2() const
{
	return schedule_sigma_hours_max2;
}
unsigned int Enviroment::get_sigma_hours_limit() const
{
	return schedule_sigma_hours_limit;
}
/*unsigned int Enviroment::get_schedule_max_hours() const
{
	return schedule_max_hours;
}*/
/*double Enviroment::get_gamma() const
{
	return GAMMA;
}*/
/*real Enviroment::get_portion() const
{
	return PORTION;
}*/

void Enviroment::select_times(Lesson& lesson,const WeekHours& week_dispon)
{
	if(lesson.subject->get_time() == 0) throw core::Exception("No puede habe hora 0 en el la materia.",__FILE__,__LINE__);
	unsigned int day_disp = week_dispon.days_disp();
	if(day_disp == 0)
	{
		lesson.week.sort(lesson.data->config);
		return;
	}
	unsigned int hours_per_day = lesson.subject->get_time() / day_disp;
	if(hours_per_day == 0) hours_per_day = 1;
	//unsigned int hours_hat = lesson.subject->get_time() - (hours_per_day * disp);

	const Time* time;
	unsigned int day;
	for(unsigned int i = 0; i < WeekHours::WEEK_SIZE - 1; i++)
	{//primer dias dsiponible no vacieo
		if(not week_dispon[i].empty())
		{
			day = i;
			break;
		}
	}
	time = &*random(week_dispon[day]);//hora base seleccionada
	for(unsigned int i = day; i < WeekHours::WEEK_SIZE - 1; i++)
	{
		week_dispon.get_day(i,hours_per_day,*time,data.config,lesson.week[i]);
		//if(lesson.week[i].size() >= lesson.subject->get_time()) break;
	}
	lesson.week.sort(lesson.data->config);
}

/*void Enviroment::random_complete_times(Lesson& lesson,const WeekOptions& week_opt)
{
	for(unsigned int i = 0; i < WeekHours::WEEK_SIZE; i++)
	{
		//std::cout << lesson.subject->get_name() << " requiere " << lesson.subject->get_time() << " de las cuales hay " << lesson.week.count_hours() << "\n";
		if(lesson.subject->get_time() <= lesson.week.count_hours()) return;
		if(lesson.week[i].size() > 0) continue;

		//TODO:realizar esta asignacion en un orden aleatorio
		week_opt[i].random(lesson.week[i]);
	}
}*/
}

