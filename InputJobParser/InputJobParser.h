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
	
public:
	InputJobParser(std::string filename);
	void GetAnalysisNames();
	void ParseOtherJobProperties(InputJob& job, const std::shared_ptr<cpptoml::table> job_properties_table);
	void ParseAnalysisCollection();
};