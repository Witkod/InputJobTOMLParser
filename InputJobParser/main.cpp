#include <iostream>
#include "cpptoml.h"
#include <filesystem>
#include "inputjob.h"
#include <typeinfo>
#include "InputJobParser.h"

int main()
{

	std::string file_name = "tbGaudi_examle.toml";
	auto parser = InputJobParser(file_name);
	parser.ParseAnalysisCollection();

	return 0;
}
