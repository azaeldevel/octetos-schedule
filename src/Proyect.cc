
#include "Proyect.hh"


namespace sche
{

void Proyect::open(const std::string& fn)
{
	oct::pack::Zip zip;
	zip.extract(fn);
	
}

}