#include <iostream>
#include <filesystem>
#include <typeinfo>
#include "cpptoml.h"
#include "inputjob.h"
#include "InputJobParser.h"

InputJobParser::InputJobParser(std::string filename) : m_source_file(filename)
{
	//trzebaby sprawdzać czy ta ścieżka wgl istnieje
	//co robić, gdy główna klasa wyrzuci wyjątek i nie sparsuje pliku?
	//lepiej, żeby konstruktor wyrzucał wyjątek i go przekazywał dalej na zewnątrz
	try
	{
		m_parsed_TOML = cpptoml::parse_file(m_source_file);
		GetAnalysisNames();
	}
	catch (const cpptoml::parse_exception& e)
	{
		std::cerr << "Failed to parse " << m_source_file << ": " << e.what() << std::endl;
	}
}

//uogólniona funkcja powinna przyjmować tabele i zwracać kontener z nazwami kluczy
void InputJobParser::GetAnalysisNames()
{
	for (const auto& analysis_pair : *m_parsed_TOML)
	{
		m_analysis_names.push_back(analysis_pair.first);
	}
}

void InputJobParser::ParseOtherJobProperties(InputJob& job, const std::shared_ptr<cpptoml::table> job_properties_table)
{
	for (const auto& property_pair_value : *job_properties_table)
	{
		if (typeid(*property_pair_value.second) == typeid(cpptoml::value<std::string>))
		{
			if (property_pair_value.first != "DutName" && property_pair_value.first != "DataPath" && property_pair_value.first != "VoltageUnit")
			{
				job.Add(property_pair_value.first, *job_properties_table->get_qualified_as<std::string>(property_pair_value.first));
			}		
		}
		else if (typeid(*property_pair_value.second) == typeid(cpptoml::value<__int64>))
		{
			if (property_pair_value.first != "RunNumber")
			{
				job.Add(property_pair_value.first, *job_properties_table->get_qualified_as<unsigned>(property_pair_value.first));
			}	
		}
		else if (typeid(*property_pair_value.second) == typeid(cpptoml::value<double>))
		{
			if (property_pair_value.first != "Voltage")
			{
				job.Add(property_pair_value.first, *job_properties_table->get_qualified_as<double>(property_pair_value.first));
			}
		}
		else if (typeid(*property_pair_value.second) == typeid(cpptoml::value<bool>))
		{
			job.Add(property_pair_value.first, *job_properties_table->get_qualified_as<bool>(property_pair_value.first));
		}
		else
		{
			std::cout << "Bad type of object " << property_pair_value.first << std::endl;
		}
	}
}

//void Parse_Input_Job

// w uogólnionej funkcji powinno tylko zmieniać wartość jeśli jest, a jeśli nie to nie- chyba?
std::string FindDataPath(std::shared_ptr<cpptoml::table_array> jobs_tables)
{
	for (const auto& job_properties_table : *jobs_tables)
	{
		auto new_data_path = job_properties_table->get_qualified_as<std::string>("DataPath");
		if (new_data_path && !(*job_properties_table).contains("DutNumber"))
		{
			return *new_data_path;
		}
		else return "";
	}
}

void InputJobParser::ParseAnalysisCollection()
{
	for (auto analysis_name : m_analysis_names) 
	{
		std::vector<InputJob> jobs;
		auto analysis_tables = m_parsed_TOML->get_table_array(analysis_name);

		// w uogólnionej funkcji powinno tylko zmieniać wartość jeśli jest, a jeśli nie to nie- chyba?
		std::string data_path = FindDataPath(analysis_tables);

		for (const auto& job_table : *analysis_tables)
		{ 
			auto dut_name = job_table->get_qualified_as<std::string>("DutName");
			auto run_number = job_table->get_qualified_as<unsigned>("RunNumber");
			auto voltage = job_table->get_qualified_as<double>("Voltage");
				
			if (dut_name && run_number && voltage)
			{
				auto job = InputJob(*dut_name, *run_number, *voltage, data_path);

				//GetDataPath()
				auto new_data_path = job_table->get_qualified_as<std::string>("DataPath");
				if (new_data_path)
				{
					job.Set_Path(*new_data_path);
				}

				ParseOtherJobProperties(job, job_table);

				//Get_Voltage_Unit()
				auto voltage_unit = job_table->get_qualified_as<std::string>("VoltageUnit");
				if (voltage_unit)
				{
					job.Set_Voltage_Unit(*voltage_unit);
				}

				//Check_Path_Existance()
				if (job.Get_Path().empty())
				{
					std::cout << "Missing data_path in " << *dut_name << " in" << analysis_name << std::endl;
				}
				else
				{
					jobs.push_back(job);
				}
			}
			else if ((*job_table).contains("DataPath")) continue;
			else  std::cout << "Lack of DutName or RunNumber or Voltage to create InputJob in " << analysis_name << std::endl;
		}
		m_analysis_collection.insert(std::pair<std::string, std::vector<InputJob>>(analysis_name, jobs));
	}
}

