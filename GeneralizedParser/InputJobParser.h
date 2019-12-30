#pragma once
#include <vector>
#include <map>
#include "InputJob.h"
#include "cpptoml.h"


class InputJobParser
{
private:
	std::string m_source_file; //toml source file
	std::shared_ptr<cpptoml::table> m_parsed_TOML;
	std::map < std::string, std ::vector<InputJob> > m_analysis_collection;
	std::vector <std::string> m_analysis_names;
	
	void ParseAnalysisCollection();
	void ParseJobProperties(InputJob& job, const std::shared_ptr<cpptoml::table> job_properties_table);
	bool CheckObligatoryValues(const std::shared_ptr<cpptoml::table>& job_properties_table, std::vector<std::string>& missing_parameters) const;
	std::string FindDataPath(std::shared_ptr<cpptoml::table_array> jobs_tables);
	
public:
	InputJobParser(std::string filename);
	void GetAnalysisNames();
	std::vector<InputJob> GetAnalysisCollection(std::string analysis_collection_name);
};
