#pragma once

#include "project_manager.hpp"
#include "common.hpp"


constexpr size_t PROJECT_NAME_MAX_LENGTH = 25;
constexpr size_t PROJECT_DESC_MAX_LENGTH = 125;

namespace pm
{
    namespace gui
    {
       
        enum class GUI_STATUS
        {
            DUPLICATE_PROJECT_NAME,
            PROJECT_CREATE_SUCCESS
        };
        
        class project_manager_gui
        {
        public:

            project_manager_gui()
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
            void on_create_project_button();
            void show_color_picker();

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

            int width_ = 600;
            int height_ = 700;

           

            project_manager project_manager_;
            //std::vector<project> active_projects_ = {};

            char project_name_[PROJECT_NAME_MAX_LENGTH]{};
            char project_desc_[PROJECT_DESC_MAX_LENGTH]{};
            std::queue<GUI_STATUS> gui_status_msgs_ = {};

            std::chrono::steady_clock::time_point popup_tp_{};
            std::chrono::microseconds popup_decay_time_ = {};
            std::chrono::microseconds popup_start_time_ = {};
            std::string popup_status_string_{};
            ImVec4 popup_text_color_ = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
            ImVec4 current_button_color_ = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

            static constexpr const char* time_format = "{:%Y/%m/%d%H:%M:%S}";

        };

        //typedef btn_size = ImVec2(ugv_path_gui::button_size_width_, ugv_path_gui::button_size_height_);
    
    }
    
}