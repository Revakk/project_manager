#include "project_manager.hpp"
#include <chrono>
#include <iostream>

void project_manager::set_active_project(const size_t _id)
{
	if (active_project_.has_value())
	{
		reset_instance_time(active_project_.value());
	}
	
	if (active_project_.has_value() && _id == active_project_.value())
	{
		std::cout << "DEACTIVATING PROJECT " << _id << '\n';
		active_project_ = std::nullopt;
		return;
	}
	active_project_ = _id;
	//after changing active project you have to reset the timer so that when you return to previous project it does not remember last_time_check(it would increase the current_instance time by a time that this project was inactive)
	for (auto& prj : projects_)
	{
		prj.last_time_check = std::chrono::high_resolution_clock::now();
	}
	std::cout << "set_active_project:" << _id << '\n';
}

std::optional<size_t> project_manager::get_active_project_id() const
{
	return active_project_;
}


void project_manager::update_time()
{
	if (!projects_.empty() && active_project_.has_value())
	{
		project& active_project = projects_.at(active_project_.value());

		auto time_now = std::chrono::high_resolution_clock::now();
		auto elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(time_now - active_project.last_time_check);
		active_project.current_instance_time_ += elapsed_time;
		active_project.overall_time_ += elapsed_time;

		active_project.last_time_check = time_now;

		//std::cout << project_time_to_string(active_project.current_instance_time_) << '\n';
	}
}

std::string project_manager::project_instance_time(const size_t _project_id)const
{
	if (!projects_.empty())
	{
		auto project = projects_.at(_project_id);
		return project_time_to_string(project.current_instance_time_);
	}

	return std::string();
}

std::string project_manager::project_overall_time(const size_t _project_id)const
{
	if (!projects_.empty())
	{
		auto project = projects_.at(_project_id);
		return project_time_to_string(project.overall_time_);
	}

	return std::string();
}

bool project_manager::add_project(const std::string _project_name,const ImVec4 _color,const std::string _desc)
{
	projects_.emplace_back(project(_project_name, std::chrono::microseconds(0), std::chrono::microseconds(0),_color,projects_.size(),_desc));
	return true;
}

std::string project_manager::project_time_to_string(std::chrono::microseconds _duration)const 
{
	auto hours = std::chrono::duration_cast<std::chrono::hours>(_duration).count();
	_duration -= std::chrono::hours(hours);
	auto minutes = std::chrono::duration_cast<std::chrono::minutes>(_duration).count();
	_duration -= std::chrono::minutes(minutes);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(_duration).count();

	return std::to_string(hours) + ":" + std::to_string(minutes) + ":" +
		std::to_string(seconds);
}

void project_manager::delete_project(const size_t _id)
{
	auto it = projects_.erase(projects_.begin()+_id);
	std::cout << _id << '\n';

	for (auto& iter = it; iter != projects_.end(); iter++)
	{
		(*it).id_ -= 1;
	}
}

void project_manager::reset_instance_time(size_t _id)
{
	projects_[_id].current_instance_time_ = std::chrono::microseconds(0);
}

std::string project_manager::get_project_string(const size_t _idx)const
{
	if (!projects_.empty())
	{
		auto prj = projects_.at(_idx);

		std::string project_text_ = "";
		project_text_ = prj.name_ + '\n';
		if (active_project_.has_value() && active_project_.value() == _idx)
		{
			project_text_ += project_instance_time(_idx) + '\n';
		}
		project_text_ += project_overall_time(_idx) + '\n';

		return project_text_;
	}
	
}
