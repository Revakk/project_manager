#pragma once
#include "project.hpp"
#include <unordered_map>
#include<span>

enum class string_specifier : uint8_t
{
	BUTTON_LABEL_ACTIVE,
	BUTTON_LABEL_INACTIVE,

};


class project_manager
{
public:
	project_manager() = default;
public:
	void set_active_project(const size_t id_);
	std::optional<size_t> get_active_project_id() const;
	void update_time();
	std::span<project> get_projects()
	{
		return {projects_.data(),projects_.size()};
	}
	std::string project_instance_time(const size_t _project_id) const;
	std::string project_overall_time(const size_t _project_id) const;
	bool add_project(const std::string _project_name,const ImVec4 _color,const std::string _desc);
	std::string get_project_string(const size_t _idx) const;
	std::string project_time_to_string(std::chrono::microseconds _duration) const;

private:
	
	void reset_instance_time(const size_t _id);

private:
	std::vector<project> projects_;
	std::optional<size_t> active_project_ = std::nullopt;
};