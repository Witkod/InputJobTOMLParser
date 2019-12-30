#include <iostream>
#include "InputJobParser.h"

InputJobParser::InputJobParser(std::string filename) : m_source_file(filename)
{
	try
	{
		m_parsed_TOML = cpptoml::parse_file(m_source_file);
		GetAnalysisNames();
		ParseAnalysisCollection();
	}
	catch (const cpptoml::parse_exception& e)
	{
		std::cerr << "Failed to parse " << m_source_file << ": " << e.what() << std::endl;
	}
}

bool InputJobParser::CheckObligatoryValues(const std::shared_ptr<cpptoml::table>& job_properties_table, std::vector<std::string>& missing_parameters) const
{
	for(auto obligatory_parameter : InputJob::obligatory_parameters)
	{
		if (obligatory_parameter.second == typeid(std::string))
		{
			const auto& obligatory_parameter_value = job_properties_table->get_qualified_as<std::string>(obligatory_parameter.first);
			if (!obligatory_parameter_value)
			{
				missing_parameters.push_back(obligatory_parameter.first);
			}
		}
		else if (obligatory_parameter.second == typeid(double))
		{
			const auto& obligatory_parameter_value = job_properties_table->get_qualified_as<double>(obligatory_parameter.first);
			if (!obligatory_parameter_value)
			{
				missing_parameters.push_back(obligatory_parameter.first);
			}
		}
		else if (obligatory_parameter.second == typeid(unsigned))
		{
			const auto& obligatory_parameter_value = job_properties_table->get_qualified_as<unsigned>(obligatory_parameter.first);
			if (!obligatory_parameter_value)
			{
				missing_parameters.push_back(obligatory_parameter.first);
			}
		}
		else if (obligatory_parameter.second == typeid(bool))
		{
			const auto& obligatory_parameter_value = job_properties_table->get_qualified_as<bool>(obligatory_parameter.first);
			if (!obligatory_parameter_value)
			{
				missing_parameters.push_back(obligatory_parameter.first);
			}
		}
		else 
		{
			std::cout << "Unrecognised parameter type " << obligatory_parameter.second.name() << " in " << obligatory_parameter.first << std::endl;
		}
	}
	if (missing_parameters.size() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void InputJobParser::GetAnalysisNames()
{
	for (const auto& analysis_pair : *m_parsed_TOML)
	{
		m_analysis_names.push_back(analysis_pair.first);
	}
}

void InputJobParser::ParseJobProperties(InputJob& job, const std::shared_ptr<cpptoml::table> job_properties_table)
{
	for (const auto& property_pair_value : *job_properties_table)
	{
		if (typeid(*property_pair_value.second) == typeid(cpptoml::value<std::string>))
		{
			if (property_pair_value.first != "DataPath")
			{
				job.SetValue(property_pair_value.first, *job_properties_table->get_qualified_as<std::string>(property_pair_value.first));
			}
		}
		else if (typeid(*property_pair_value.second) == typeid(cpptoml::value<std::int64_t>))
		{
			job.SetValue(property_pair_value.first, *job_properties_table->get_qualified_as<unsigned>(property_pair_value.first));
		}
		else if (typeid(*property_pair_value.second) == typeid(cpptoml::value<double>))
		{

			job.SetValue(property_pair_value.first, *job_properties_table->get_qualified_as<double>(property_pair_value.first));
		}
		else if (typeid(*property_pair_value.second) == typeid(cpptoml::value<bool>))
		{
			job.SetValue(property_pair_value.first, *job_properties_table->get_qualified_as<bool>(property_pair_value.first));
		}
		else
		{
			std::cout << "Bad type of object " << property_pair_value.first << std::endl;
		}
	}
}

std::string InputJobParser::FindDataPath(std::shared_ptr<cpptoml::table_array> jobs_tables)
{
	std::vector<std::string> missing_parameters;
	for (const auto& job_properties_table : *jobs_tables)
	{
		auto new_data_path = job_properties_table->get_qualified_as<std::string>("DataPath");
		CheckObligatoryValues(job_properties_table, missing_parameters);
		if (new_data_path && missing_parameters.size() == InputJob::obligatory_parameters.size())
		{
			return *new_data_path;
		}
	}
	return "";
}

void PrintMissingParameters(const std::string analysis_name, std::vector<std::string>& missing_parameters)
{
	std::string missing_parameters_information("Lack of parameters in one of jobs: ");
	for(auto missing_parameter : missing_parameters)
	{
		missing_parameters_information += missing_parameter + " ";
	}
	missing_parameters_information += "in " + analysis_name;
	std::cout << missing_parameters_information << std::endl;
}

void InputJobParser::ParseAnalysisCollection()
{
	for (auto analysis_name : m_analysis_names) 
	{
		std::vector<InputJob> jobs;
		auto analysis_table = m_parsed_TOML->get_table_array(analysis_name);

		std::string global_data_path_in_analysis = FindDataPath(analysis_table);

		for (const auto& job_table : *analysis_table)
		{ 
			std::vector<std::string> missing_parameters;
			if (CheckObligatoryValues(job_table, missing_parameters))
			{
				auto job = InputJob();

				auto new_data_path = job_table->get_qualified_as<std::string>("DataPath");
				if (new_data_path)
				{
					job.Set_Path(*new_data_path);
				}
				else
				{
					job.Set_Path(global_data_path_in_analysis);
				}
				ParseJobProperties(job, job_table);

				if (job.Get_Path().empty())
				{
					std::cout << "Missing data_path in one of jobs in " << analysis_name << std::endl;
				}
				else
				{
					jobs.push_back(job);
				}
			}
			else if ((*job_table).contains("DataPath") && missing_parameters.size() == InputJob::obligatory_parameters.size()) continue;
			else  PrintMissingParameters(analysis_name, missing_parameters);
		}
		m_analysis_collection.insert(std::pair<std::string, std::vector<InputJob>>(analysis_name, jobs));
	}
}

std::vector<InputJob> InputJobParser::GetAnalysisCollection(std::string analysis_collection_name)
{
	return m_analysis_collection[analysis_collection_name];
}
