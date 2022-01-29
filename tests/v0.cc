
#include <CUnit/Basic.h>
#include <iostream>
#include <cstdlib>
#include <schedule.hh>
#include <locale>
#include <ctime>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>
#include <csignal>
#include <filesystem>


#include <Packing.hh>
#include <Project.hh>



void schedule_devel();
int schedule_init(void);
int schedule_clean(void);

void time_devel()
{

	oct::ec::sche::Time times1[10];
	for(unsigned int i = 0; i < 10; i++)
	{
	    times1[i].read("Tue 05:00","%a %H:%M");
		/*times1[i].print(std::cout,"%c");
		std::cout << ", " << (std::time_t)times1[i];
		std::cout << ", " << &times1[i];
		std::cout << "\n";*/
	}
	//std::cout << "\n";
	for(unsigned int i = 1; i < 10; i++)
	{
		if(times1[0] == times1[i])
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times1[i].tm_wday == 2)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times1[i].tm_mday == 6)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times1[i].tm_year == 70)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}
	for(unsigned int i = 1; i < 10; i++)
	{
		if(times1[0] != times1[i])
		{
			CU_ASSERT(false);
		}
		else
		{
			CU_ASSERT(true);
		}
	}
	if(times1[0].tm_hour == 5 and times1[0].tm_min == 0)
	{
		CU_ASSERT(true);
	}
	else
	{
		CU_ASSERT(false);
	}
	for(unsigned int i = 1; i < 10; i++)
	{
		times1[i].add(i * 45 * 60);
	}
	/*for(unsigned int i = 0; i < 10; i++)
	{
		times1[i].print(std::cout,"%c");
		std::cout << ", " << (std::time_t)times1[i];
		std::cout << "\n";
	}
	std::cout << "\n";*/
	for(unsigned int i = 1; i < 10; i++)
	{
		if(times1[i-1] < times1[i])
		{
			CU_ASSERT(true);
		}
		else
		{
			times1[i-1].print(std::cout,"%a %H:%M");
			std::cout << "<";
			times1[i].print(std::cout,"%a %H:%M");
			CU_ASSERT(false);
		}
		if(times1[i].tm_wday == 2)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times1[i].tm_mday == 6)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times1[i].tm_year == 70)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}
	for(unsigned int i = 0; i < 9; i++)
	{
		if(times1[i+1] > times1[i])
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}
	for(unsigned int i = 1; i < 10; i++)
	{
		if(times1[i-1] <= times1[i])
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}
	for(unsigned int i = 9; i > 0; i--)
	{
		if(times1[i] > times1[i-1])
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}


	oct::ec::sche::Time times2[10];
	for(unsigned int i = 0; i < 10; i++)
	{
	    times2[i].read("Wed 05:00","%a %H:%M");
		/*times2[i].print(std::cout,"%c");
		std::cout << ", " << (std::time_t)times2[i];
		std::cout << "\n";*/
	}
	for(unsigned int i = 1; i < 10; i++)
	{
		if(times2[0] == times2[i])
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times2[i].tm_wday == 3)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times2[i].tm_mday == 7)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times2[i].tm_year == 70)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}
	for(unsigned int i = 1; i < 10; i++)
	{
		times2[i].add(i*45*60);
	}
	for(unsigned int i = 1; i < 10; i++)
	{
		if(times2[i].tm_wday == 3)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times2[i].tm_mday == 7)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
		if(times2[i].tm_year == 70)
		{
			CU_ASSERT(true);
		}
		else
		{
			CU_ASSERT(false);
		}
	}
	/*std::cout << "\n";
	for(unsigned int i = 0; i < 10; i++)
	{
		times2[i].print(std::cout,"%c");
		std::cout << ", " << (std::time_t)times2[i];
		std::cout << "\n";
	}*/
}

void zip_devel()
{
    std::filesystem::path oring_file = "../../tests/project";
    std::filesystem::path compressed_directory = "compress";
    std::filesystem::path compressed_file = compressed_directory / "project.sche";
    std::filesystem::path extracted_file = compressed_directory / "project.extract";
    std::filesystem::path path_origin = oring_file;
    /*std::cout << "lexically_normal : " << path_origin.lexically_normal() << "\n";
    std::cout << "parent_path : " << path_origin.parent_path() << "\n";
    std::cout << "root_directory : " << path_origin.root_directory() << "\n";
    std::cout << "absolute : " << std::filesystem::absolute(path_origin) << "\n";
    std::cout << "fn : " << compressed_file.filename() << "\n";
    */
    /*
    for (auto const& dir_entry : std::filesystem::directory_iterator{oring_file})
    {
        std::cout << "origin : " << dir_entry << "\n";
    }
    */

    if(std::filesystem::exists(compressed_file)) std::filesystem::remove(compressed_file);

	oct::pack::Zip zip;
	if(not std::filesystem::exists(compressed_directory)) std::filesystem::create_directory(compressed_directory);
	zip.compress(oring_file,compressed_file);

	if(std::filesystem::exists(compressed_file))
    {
        CU_ASSERT(true);
    }
    else
    {
        CU_ASSERT(false);
    }


    if(std::filesystem::exists(extracted_file))std::filesystem::remove_all(extracted_file);
	if(not std::filesystem::exists(extracted_file)) std::filesystem::create_directory(extracted_file);
	//std::cout << "extracted_file = " << extracted_file << "\n";
    zip.extract(compressed_file,extracted_file);

}

void project_devel()
{
    oct::core::Shell shell;
    std::string oring_file = "../../tests/project";
    std::string compressed_directory = "compress";
    std::string compressed_file = compressed_directory + "/project-sche.sche";
    std::string extracted_file = compressed_directory + "/project-sche.extract";

    if(not shell.exists(compressed_directory)) shell.mkdir(compressed_directory);
    if(shell.exists(compressed_file)) shell.rm(compressed_file);
    if(shell.exists(extracted_file)) shell.rm(extracted_file,true);

    sche::Project project;
    project.save(oring_file,compressed_file);
    if(shell.exists(compressed_file))
   	{
        CU_ASSERT(true);
    }
    else
    {
        CU_ASSERT(false);
    }

    project.open(compressed_file,extracted_file);
    if(shell.exists(extracted_file))
   	{
        CU_ASSERT(true);
    }
    else
    {
        CU_ASSERT(false);
    }
}
int main(int argc, char *argv[])
{
	/* initialize the CUnit test registry */
	if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();

	#if defined(__GNUC__) && defined(__linux__)
	signal(SIGSEGV,oct::core::signal_segmentv);
	signal(SIGABRT,oct::core::signal_abort);
    #elif defined(__GNUC__) && (defined(_WIN32) || defined(_WIN64))

    #else
        #error "Pltaforma desconocida"
    #endif

	CU_pSuite pSuite = NULL;
	pSuite = CU_add_suite("Testing Evolution Computing..", schedule_init, schedule_clean);
	if (NULL == pSuite)
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	if ((NULL == CU_add_test(pSuite, "Developing Time class", time_devel)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite, "Developing Zip class", zip_devel)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	/*if ((NULL == CU_add_test(pSuite, "Developing Project class", project_devel)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}*/

	if ((NULL == CU_add_test(pSuite, "Developing Schedule", schedule_devel)))
	{
		CU_cleanup_registry();
		return CU_get_error();
	}


	/* Run all tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
