#pragma once
#include <iostream>
#include <typeindex>
#include <map>
#include <any>

class InputJob
{
friend class InputJobParser;
private:
	std::string m_data_path;
	std::map<std::string, std::any> m_parameters;

	static std::map<std::string, std::type_index> obligatory_parameters;
	static std::map<std::string, std::string> default_units;

public:
	InputJob();
	template<typename Type> static void AddObligatoryParameter(std::string parameter_name);
	bool Exists(const std::string& parameter);
	void SetValue(const std::string& key, std::any value);
	template <typename T> T GetValue(const std::string& parameter_name);
	void Set_Path(std::string data_path);
	std::string Get_Path();
	static void AddDefaultUnit(std::string unit, std::string unit_value);
	void PrintDetails();
};


template<typename Type>static void InputJob::AddObligatoryParameter(std::string parameter_name)
{
	obligatory_parameters.insert(std::make_pair(parameter_name, std::type_index(typeid(Type))));
}

template <typename T> T InputJob::GetValue(const std::string& parameter_name)
{
	try
	{
		if (Exists(parameter_name))
		{
			return std::any_cast<T>(m_parameters[parameter_name]);
		}
		else
		{
			std::cout << "Parameter:  " << parameter_name << " not exist." << std::endl;
			// ERROR: The specified parameter is not defined 
			// trzeba to jakoœ obs³u¿yæ ?    }    
			return T{}; // zwracamy '0' danego typu
		}
	}
	catch (const std::bad_any_cast& e)
	{
		std::cout << e.what() << std::endl;
		return T{};
	}
}