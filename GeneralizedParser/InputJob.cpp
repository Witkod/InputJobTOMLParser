#include "InputJob.h"

std::map<std::string, std::type_index> InputJob::obligatory_parameters;
std::map<std::string, std::string> InputJob::default_units;

InputJob::InputJob()
{
	for (auto default_unit : InputJob::default_units)
	{
		std::string key = default_unit.first + "Unit";
		SetValue(key, default_unit.second);
	}
}

bool InputJob::Exists(const std::string& parameter)
{
	return m_parameters.find(parameter) != m_parameters.end() ? true : false;
}


void InputJob::SetValue(const std::string& key, std::any value)
{
	if (Exists(key))
	{
		if (m_parameters[key].type() != value.type()) {
			std::cout << "Bad type of parameter with key: " << key << std::endl;
			// ERROR: The specified parameter is of wrong type value      
			// trzeba to jakoœ obs³u¿yæ...     
		}
		else
		{
			m_parameters[key] = value;
		}
	}
	else
	{
		m_parameters.emplace(key, value);
	}
}

void InputJob::Set_Path(std::string data_path)
{
	m_data_path = data_path;
}

std::string InputJob::Get_Path()
{
	return m_data_path;
}

void InputJob::AddDefaultUnit(std::string unit, std::string unit_value)
{
	InputJob::default_units.insert(std::make_pair(unit, unit_value));
}

void InputJob::PrintDetails()
{
	for (auto& [key, val] : m_parameters)
	{
		if (val.type() == typeid(std::string))
			std::cout << key << ": " << std::any_cast<std::string>(val) << std::endl;
		else if (val.type() == typeid(unsigned))
			std::cout << key << ": " << std::any_cast<unsigned>(val) << std::endl;
		else if (val.type() == typeid(double))
			std::cout << key << ": " << std::any_cast<double>(val) << std::endl;
		else if (val.type() == typeid(bool))
			std::cout << key << ": " << std::any_cast<bool>(val) << std::endl;
		else
			std::cout << "Unrecognised type in iput job property: " << key << std::endl;
	}
	std::cout << "DataPath: " << m_data_path << std::endl;
}