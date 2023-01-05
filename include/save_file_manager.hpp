#pragma once
#include <nlohmann/json.hpp>
#include <fstream>
#include <optional>
#include <type_traits>

namespace pm::save_file
{
	bool is_empty(std::ifstream& file_name)
	{
		return file_name.peek() == std::ifstream::traits_type::eof();
	}


	std::optional<nlohmann::json> read_config(std::string _config_name)
	{
		std::ifstream read_config;

		read_config.open(_config_name);

		if (read_config.is_open())
		{
			if (is_empty(read_config))
			{
				return;
			}
			std::string json_config_ss;
			
			nlohmann::json config_js;
			read_config >> config_js;

			read_config.close();
			return config_js;
		}
		else
		{
			std::ofstream write_config;

			write_config.open(_config_name);

			write_config.close();
			return std::nullopt;
		}

	}

	bool write_config(nlohmann::json& _config_js,std::string _config_name)
	{
		std::ofstream write_config;

		write_config.open(_config_name);

		if (write_config.is_open() && !_config_js.empty())
		{
			write_config << std::setw(4) << _config_js;
			write_config.close();
			return true;
		}
		else
		{
			return false;
		}
	}


	template<typename Project>
	std::vector<std::pair<Project, std::chrono::seconds>> config_projects()
	{

		std::vector<std::pair<Project, std::chrono::seconds>> active_projects;
		if (config_js.empty())
		{
			std::cout << "json empty";
			return active_projects;
		}
		active_projects.reserve(config_js.size());


		for (auto& a : config_js.items())
		{
			active_projects.emplace_back(std::make_pair(a.key(), a.value()));
		}


		return active_projects;
	}

	template<typename Project>
	nlohmann::json vector_to_json(std::vector<Project> _active_projects)
	{
		static_assert(std::is_convertible_v<Project, nlohmann::json>, "Project type is not convertible to nlohmann::json");

		std::vector<nlohmann::json> jsons;
		for (auto&& prj : _active_projects)
		{
			nlohmann::json js = prj;
			jsons.emplace_back(js);
		}
		nlohmann::json output_js = jsons;

		return output_js;
	}


	std::string pretty_print(std::chrono::seconds& sec)
	{
		std::string hours = std::to_string(std::chrono::duration_cast<std::chrono::hours>(sec).count());
		std::string minutes = std::to_string(std::chrono::duration_cast<std::chrono::minutes>(sec).count() % 60);
		std::string seconds = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(sec).count() % 60);

		std::string whole_string = hours + ":" + minutes + ":" + seconds;

		return whole_string;
	}


	void export_to_csv(nlohmann::json _config_js)
	{
		std::ofstream csv_file;

		csv_file.open("projects.csv");

		csv_file << "Project name; Time created ; Time spent on project ; Description \n";
		std::chrono::seconds sec(0);

		for (nlohmann::json::iterator it = _config_js.begin(); it != _config_js.end(); ++it)
		{
			std::string project_name = it->begin().key();
			nlohmann::json value = it->begin().value();
			sec = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::microseconds(value.at("project_time")));
			auto time_created = std::chrono::system_clock::to_time_t(value.at("time_created"));
			csv_file << project_name << ";" << pretty_print(sec) << ";\n";
		}
		csv_file.close();
	}
}