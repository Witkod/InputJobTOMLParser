#include <iostream>
#include "cpptoml.h"
#include <filesystem>

//#include <cassert>

//namespace fs = std::filesystem;


int main(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " filename" << std::endl;
		return 1;
	}

	try
	{
		std::shared_ptr<cpptoml::table> g = cpptoml::parse_file(argv[1]);
		std::string my_path_string = *g->get_as<std::string>("path");
		std::filesystem::path my_path(my_path_string);
		//std::filesystem::path my_path = "C:\PW_private";
		if (std::filesystem::exists(my_path)) {
			std::cout << "Loaded path does not exist." << std::endl;
		}
		else std::cout << my_path;

	}
	catch (const cpptoml::parse_exception& e)
	{
		std::cerr << "Failed to parse " << argv[1] << ": " << e.what() << std::endl;
		return 1;
	}
	

	return 0;
}
