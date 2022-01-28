

#ifndef OCTETOS_PACKING_HH
#define OCTETOS_PACKING_HH

#include <string>
#include <zip.h>
#include <octetos/core/shell.hh>

namespace oct::pack
{


class Package
{
public:
	virtual void compress(const std::string& source,const std::string& dest) = 0;
	virtual void extract(const std::string& source,const std::string& dest) = 0;
	//virtual void add(const std::string& source) = 0;

protected:
	static bool is_directory(const std::string&);
};


class Zip : public Package
{
public:
	Zip();
	
	virtual void compress(const std::string& source,const std::string& dest);
	virtual void extract(const std::string& source,const std::string& dest);
	virtual void add(const std::string& source,const std::string&);

private:
	void compres_walk_directory(const std::string& source);
	
	zip_t* zipper;
	oct::core::Shell shell;
};



#endif
} 
