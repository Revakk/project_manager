#pragma once
#include "project.hpp"
#include <unordered_map>

class project_manager
{
public:
	project_manager() = default;
public:
	void set_active_project(const int id_);
	size_t get_active_project_id() const;
	void update_times();
	std::vector<project>& get_projects()
	{
		return projects_;
	}

private:
	std::vector<project> projects_;
	size_t active_project_ = 0;
};