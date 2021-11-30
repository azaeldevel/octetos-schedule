

#ifndef OCTETOS_SCHEDULE_HH
#define OCTETOS_SCHEDULE_HH

#include <string>
#include <list>
#include <vector>
#include <ctime>

namespace oct::core
{
	class Person
	{
	public:
		Person(const std::string& name,const std::string& ap,const std::string& am);
		Person(const std::string& name);
		Person();

		const Person& operator =(const std::string& name);
		virtual void get_name(std::string&);

	private:
		std::vector<std::string> names; 
	};
}

namespace ec::sche
{
	struct Time
	{
		tm begin;
		tm end;

		Time()
		{
			begin = {0};
			end = {0};
		}
	};

	class Target
	{
	public:
		virtual const std::string& get_name() = 0;
	};

	struct Fragment
	{
		const Target* target;
		Time time;
	};
}

namespace oct::sche
{

	class Configuration
	{
	public:
		enum SchemaWeek
		{
			MF,//Mondy - Fraday
			MS,//Monday - Sturday
		};
	public:
		Configuration(const std::string& name);

	private:
		SchemaWeek schema;
		
	};


	enum Day
	{
		Sunday,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};
		
	

	class Teacher : public oct::core::Person, public ec::sche::Target
	{
	public:
		Teacher(const std::string& name,const std::string& ap,const std::string& am);
		Teacher(const std::string& name);
		Teacher();
		
		virtual const std::string& get_name();
		
	private:
		std::string name;
	};
		
	class Subject : public ec::sche::Target
	{
	public:
		Subject(const std::string& name);
		Subject();

		virtual const std::string& get_name();
		
	private:
		std::string name;
		Day day;
	};
	
	class Room : public ec::sche::Target
	{
	public:
		Room(const std::string& name);
		Room();

		virtual const std::string& get_name();
		
	private:
		std::string name;
	};
	
	
	
	
	
	class Teachers
	{
		struct Row : public std::vector<ec::sche::Time>
		{
			Teacher teacher;

			Row();
			Row(int z);		
		};
		
	public: 
		Teachers(const std::string& fn);
		void loadFile(const std::string& fn);
		void print(std::ostream&);

	private:
		std::list<Row> teachers;
	};

	class Subjects
	{
		struct Row
		{
			Subject subject;
			Teacher teacher;

			Row();
		};
		
	public: 
		Subjects(const std::string& fn);
		void loadFile(const std::string& fn);
	};

	class Rooms
	{
		struct Row : public std::vector<ec::sche::Time>
		{
			Room room;

			Row();
			Row(int z);		
		};
		
	public:
		Rooms(const std::string& fn);
		void loadFile(const std::string& fn);
	private:
		std::list<Row> rooms;
	};
				
}

#endif