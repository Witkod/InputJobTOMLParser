#pragma once
#include <iostream>

class InputJob
{
private:
	//inputJob ma mieæ w sobie œcie¿ke i Unit tylko, ¿e w domyœle Unit definiujemy 'V', ale jeœli jest zdefiniowany w runie to nadpisujemy DONE
	//dodaæ do konstruktora DataPath DONE
	
	unsigned m_run_number;
	std::string m_dut_name;
	double m_run_volatage;
	std::string m_run_voltage_unit;
	std::string m_data_path;

	std::map<std::string, double>  m_double_keys;
	std::map<std::string, unsigned> m_unsigned_keys;
	std::map<std::string, bool> m_bool_keys;
	std::map<std::string, std::string>  m_string_keys;

	std::vector<std::string> m_obligatory_string_keys;
	std::vector<std::string> m_obligatory_double_keys;
	std::vector<std::string> m_obligatory_unsigned_keys;
	std::vector<std::string> m_obligatory_bool_keys;

public:
	InputJob(std::string dut_name, unsigned run_number, double run_voltage,
			std::string data_path, std::string run_voltage_unit = "V" ):
			m_dut_name(dut_name), m_run_volatage(run_voltage), m_run_number(run_number),
			m_data_path(data_path), m_run_voltage_unit(run_voltage_unit)
	{
	}

	void AddObligatoryStringKey(std::string key_name)
	{
		m_obligatory_string_keys.push_back(key_name)
	}

	void AddObligatoryDoubleKey(std::string key_name)
	{
		m_obligatory_double_keys.push_back(key_name)
	}

	void AddObligatoryUnsignedKey(std::string key_name)
	{
		m_obligatory_unsigned_keys.push_back(key_name)
	}

	void AddObligatoryBoolKey(std::string key_name)
	{
		m_obligatory_bool_keys.push_back(key_name)
	}

	void Set_Path(std::string data_path)
	{
		m_data_path = data_path;
	}

	void Set_Voltage_Unit(std::string run_voltage_unit)
	{
		m_run_voltage_unit = run_voltage_unit;
	}

	std::string Get_Path()
	{
		return m_data_path;
	}

	void Add(const std::string key, std::string value)
	{
		m_string_keys.insert(std::pair<std::string, std::string>(key, value));
	}

	void Add(const std::string key, unsigned value)
	{
		m_unsigned_keys.insert(std::pair<std::string, unsigned>(key, value));
	}

	void Add(const std::string key, double value)
	{
		m_double_keys.insert(std::pair<std::string, double>(key, value));
	}

	void Add(const std::string key, bool value)
	{
		m_bool_keys.insert(std::pair<std::string, bool>(key, value));
	}
};
