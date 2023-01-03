#ifndef PROJECT_HPP
#define PROJECT_HPP

#include "common.hpp"
#include "utilities.hpp"
#include <nlohmann/json.hpp>

constexpr unsigned int number_of_colours = 4;

inline std::array<ImVec4, number_of_colours> color_array = {
    ImVec4(0.275f,0.5f,0.95f,1.0f),
    ImVec4(0.2f,0.698f,0.286f,1.0f),
    ImVec4(1.0f,0.741f,0.012f,1.0f),
    ImVec4(0.866f,0.474f,0.450f,1.0f)
};

inline ImVec4 random_color()
{
    return(color_array[rngGen(0, number_of_colours-1)]);
}

struct project
{
    std::string name_ = "";
    std::chrono::microseconds current_instance_time_ = {};
    std::chrono::microseconds overall_time_ = {};
    ImVec4 color_ = random_color();
    size_t id_ = 0;
    std::string description_ = "";
    std::chrono::time_point<std::chrono::system_clock> time_created = std::chrono::system_clock::now();
    std::chrono::time_point<std::chrono::high_resolution_clock> last_time_check = std::chrono::high_resolution_clock::now();
    bool currently_active_ = false;

    nlohmann::json parse_into_json()
    {
        nlohmann::json outer_js;
        nlohmann::json inner_js;

        inner_js["project_time"] = overall_time_.count();
        inner_js["color"] = { color_.x,color_.y ,color_.z ,color_.w };
        inner_js["id"] = id_;
        inner_js["description"] = description_;
        inner_js["time_created"] = time_created.time_since_epoch().count();

        outer_js[name_] = inner_js;
        return outer_js;
    }
};

enum class PROJECT_ACTION
{
    CREATE,
    EDIT,
    DELETE
};

#endif