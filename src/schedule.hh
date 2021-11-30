

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

	private:
		std::list<std::string> names; 
	};
}


namespace oct::sche
{

	class Configuration
	{
	public:
		enum Schema
		{
			MF,//Mondy - Fraday
			MS,//Monday - Sturday
		};
	public:
		Configuration(const std::string& name);

	private:
		Schema schema;
		
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
		
	class Teacher : public oct::core::Person
	{
	public:
		Teacher(const std::string& name,const std::string& ap,const std::string& am);
		Teacher(const std::string& name);
		Teacher();

		
	private:
		
	};
	
	class Subject
	{
	public:
		Subject(const std::string& name);
		Subject();

	private:
		std::string name;
		Day day;
	};
	
	class Room
	{
	public:
		Room(const std::string& name);
		Room();

	private:
		std::string name;
	};
	
	class Row 
	{

	};
	
	class Teachers
	{
		struct Time
		{
			tm begin;
			tm end;
		};
		struct Row : public std::vector<Teachers::Time>
		{			
			Teacher teacher;

			Row();
			Row(int z);		
		};
		
	public: 
		Teachers(const std::string& fn);
		bool loadFile(const std::string& fn);

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
		bool loadFile(const std::string& fn);
	};

	class Rooms
	{		
		struct Time
		{
			tm begin;
			tm end;
		};
		struct Row : public std::vector<Rooms::Time>
		{			
			Room room;

			Row();
			Row(int z);		
		};
	public:
		Rooms(const std::string& fn);
		bool loadFile(const std::string& fn);
	private:
		std::list<Row> rooms;
	};
				
}

#endif