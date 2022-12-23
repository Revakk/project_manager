#pragma once
#include "project.hpp"
#include <unordered_map>
#include<span>

class project_manager
{
public:
	project_manager() = default;
public:
	void set_active_project(const int id_);
	std::optional<size_t> get_active_project_id() const;
	void update_time();
	std::span<project> get_projects()
	{
		return {projects_.data(),projects_.size()};
	}
	std::string project_instance_time(size_t _project_id);
	std::string project_overall_time(size_t _project_id);
	bool add_project(std::string _project_name,ImVec4 _color);

private:
	std::string project_time_to_string(std::chrono::microseconds _duration);

private:
	std::vector<project> projects_;
	std::optional<size_t> active_project_ = std::nullopt;
};