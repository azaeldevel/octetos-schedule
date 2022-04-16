

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


#ifndef EC_SCHEDULE_HH
#define EC_SCHEDULE_HH


#include "schedule-data.hh"



namespace oct::ec::sche
{
	class Enviroment;


	typedef unsigned int Child;

	/**
	*\brief Representa una posible solucion a nuestro problema
	*/
	class Single : public oct::ec::Single, public Schedule
	{

	public:
		Single(Enviroment& env,unsigned int);
		/**
		*\brief Constructor que recive el ID del Individuo y el Ambiente
		*/
		//Single(ID id,Enviroment& env, const Schedule&);
		Single(Enviroment& env);

		/**
		*\brief Evalua al individuo y asigna su valor de adaptabilidad(fitness)
		*/
		virtual void eval();


		/**
		*\brief Crea un arcivo CSV con los datos relevantes del individuo
		*/
		virtual void save(Save& fn);
		/**
		*\brief Realiza el apareo entre dos individuos
		*/
		virtual void juncting(std::list<oct::ec::Single*>& chils,const oct::ec::Single* single);
		/**
		*\brief Imprime los datos relevantes del individuo
		*/
		virtual void print(std::ostream&) const;

		virtual void mutate();

	public:

		static const unsigned int WEEK_HOURS;
		static const unsigned int WEEK_HOURS2;
		static const unsigned int WEEK_HOURS3;
		static unsigned int jump_saver;

	private:

		/**
		*\brief Retorna la cantiad de traslapes que tiene los maestros
		*/
		void overlap_by_teacher();
		/**
		*\brief Retorna suma de la diferacia de horas entre los horasio asignados para cada materia y las horas de cada materia
		*/
		void cover();

		void not_empty();

		/**
		*\brief Asegura que las horas selecionas este dentro de un rango de desviacion esandar asignado
		*/
		void sigma_hours();
		real sigma_hours(const ClassRoom&);
		real sigma_hours(const Lesson&);


		/**
		*\brief Asegura que si la clase es de mas de una horas sean continuas
		*/
		void unique_class();//TODO: implementar

		/**
		*\brief Cuando los horaios no son continuos la interseccion no produce los resutados esperados, esta funcion esta pensada realizar una verificacion de esta situacion
		*/
		// un ejemplo seria el horario de un salon con una hora de receso difernete al la hora de una clase nommal,
		// en esta caso al asignar un profeso que puede cubir desde el inico del dia hasta un horario cualquiere que sobrepase el recreo
		// el horario del maestro despues de recreo no coincidira con el horario del salon despues del recreo al no condicerar en el horario
		// del maestro este salto en el tiempo.
		bool bound_schedule()const; //TODO:implemetar
		
	private:
		typedef void (Schedule::*algorit)(const Schedule&,const Schedule&);

		algorit random_algorit();
	};

	/**
	*\brief Variables de control y proceso
	*/
	class Enviroment : public oct::ec::Enviroment
	{
	public:

		/**
		*\brief Constructor que recive un directorio para guardar estadisticas
		*/
		Enviroment(const std::filesystem::path& log,const std::filesystem::path& project_dir,const std::filesystem::path& output_dir);
		Enviroment();

		/**
		*\brief Inicia las variables
		*/
		void init(const std::filesystem::path&);

		void init(const std::filesystem::path& log,const std::filesystem::path& project_dir,const std::filesystem::path& output_dir);

		~Enviroment();

		const Data& get_data()const;

		unsigned int get_criterion()const;
		unsigned int get_overlap_max() const;
		unsigned long get_overlap_max2() const;
		unsigned int get_cover_max() const;
		unsigned long get_cover_max2() const;
		unsigned int get_empty_max() const;
		unsigned int get_sigma_hours_max() const;
		unsigned int get_sigma_hours_max2() const;
		unsigned int get_sigma_hours_limit() const;

		/**
		*\brief Inicia el proceso de apareo, sobecragada devido a que deve distigir entre grupo para realizar el apareoa
		*/
		//virtual void juncting();

		/**
		*\brief Crea la poblacion inicial
		*/
		virtual void initial();

		void select_times(Lesson&,const WeekHours&);

	private:
		//std::filesystem::path input_directory;
		Data data;
		std::filesystem::path strid;

		unsigned int CRITERION;
		unsigned int schedule_overlap_max;
		unsigned long schedule_overlap_max2;
		unsigned int schedule_cover_max;
		unsigned long schedule_cover_max2;
		unsigned int SCHEDULE_EMPTY_MAX;
		real schedule_sigma_hours_max;
		real schedule_sigma_hours_max2;
		real schedule_sigma_hours_limit;
	};

}

#endif
