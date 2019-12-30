#include <iostream>
#include "InputJobParser.h"

int main()
{

	std::string file_name = "tbGaudi_examle.toml";

	InputJob::AddObligatoryParameter<std::string>("DutName");
	InputJob::AddObligatoryParameter<unsigned>("RunNumber");
	InputJob::AddObligatoryParameter<double>("Voltage");
	InputJob::AddDefaultUnit("Voltage", "mV");
	auto parser = InputJobParser(file_name);

	auto analysis_collection = parser.GetAnalysisCollection("analysis1");
	analysis_collection.front().PrintDetails();
	auto voltage_unit = analysis_collection.front().GetValue<std::string>("VoltageUnit");
	std::cout << "Read parameter: " << voltage_unit << std::endl;
	return 0;
}
