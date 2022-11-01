#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vector>
#include <array>
#include <queue>
#include <chrono>


constexpr size_t PROJECT_NAME_MAX_LENGTH = 25;

namespace lpp
{
    namespace ugv
    {
        enum class PROJECT_ACTION
        {
            CREATE,
            EDIT,
            DELETE
        };

        enum class GUI_STATUS
        {
            DUPLICATE_PROJECT_NAME,
            PROJECT_CREATE_SUCCESS
        };


        struct project
        {
            std::string name_ = "";
            std::time_t current_instance_time_ = {};
            std::time_t overall_time_ = {};
            ImVec4 color_{};
            
            bool currently_active_ = false;

        };

        
        class ugv_path_gui
        {
        public:

            ugv_path_gui()
            {
                
            }

            void init(GLFWwindow* _window, const char* _glsl_version,const int& _width, const int& _height);

            void update();

            void new_frame();

            void render_ui();

            void shutdown();

        private:
            void render_button_section();

            void render_project_section();

            void on_create_project();
            void on_edit_project();
            void on_export_time();

            void export_time_popup();
            void edit_project_popup();
            void create_project_popup();

            void project_list_renderer();
            void status_msg_popup();

            void render_popup_message();

            void on_project_button_active(const std::string_view& _project_name);

        private:
            const float button_size_width_ = 125.0f;
            const float button_size_height_ = 50.0f;

            const float button_project_width_ = 120.0f;
            const float button_project_height_ = 80.0f;

        private:

            int window_flags_ = 0;

            bool create_project_ = false;
            bool edit_project_ = false;
            bool export_time_ = false;

            int width_;
            int height_;

            std::vector<project> active_projects_ = {};

            char project_name_[PROJECT_NAME_MAX_LENGTH]{};
            std::queue<GUI_STATUS> gui_status_msgs_ = {};

            std::time_t popup_decay_time_ = std::time_t{0};
            std::time_t popup_start_time_ = std::time_t{ 0 };
            std::string popup_status_string_{};
            ImVec4 popup_text_color_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);


        };

        //typedef btn_size = ImVec2(ugv_path_gui::button_size_width_, ugv_path_gui::button_size_height_);
    
    }
    
}