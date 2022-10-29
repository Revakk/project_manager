#include "ugv_gui.hpp"
#include <chrono>
#include <functional>
#include <imgui.h>
#include <implot.h>
#include <memory>
#include <thread>
#include <iostream>

namespace lpp
{
    namespace ugv
    {
        void ugv_path_gui::init(GLFWwindow *_window, const char *_glsl_version,const int& _width, const int& _height)
        {
            width_ = _width;
            height_ = _height;

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImPlot::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            io.Fonts->AddFontFromFileTTF("fonts/active_font.ttf", 15);

            //io.DeltaTime = 10.0f;
           // std::thread path_controller_thread();

            ImGui_ImplGlfw_InitForOpenGL(_window, true);
            ImGui_ImplOpenGL3_Init(_glsl_version);
            ImGui::StyleColorsDark();

            
            //ImGuiStyleVar_FrameRounding;
        }


        void ugv_path_gui::render_button_section()
        {
            //ImGui::SetCursorPos(ImVec2(20, 20));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7, 0.8, 0.8, 0.3));

            //ImGui::SetCursorPosX(((width_)-width_/1.33333f) - (button_size_width_));
            ImGui::SetCursorPosX((width_*0.25f) - (button_size_width_/2.0f));
            ImGui::SetCursorPosY(20);
            if (ImGui::Button("CREATE PROJECT", ImVec2(button_size_width_, button_size_height_)))
            {
                create_project_ = true;
                on_create_project();
            }
            
            //ImGui::SetCursorPosX(((width_)-width_ / 2.0f) - (button_size_width_ / 2.0f));
            ImGui::SetCursorPosX((width_ * 0.5f) - (button_size_width_ / 2.0f));
            ImGui::SetCursorPosY(20);
            if (ImGui::Button("EXPORT TIMES", ImVec2(button_size_width_, button_size_height_)))
            {
                on_export_time();
            }

            //ImGui::SetCursorPosX(((width_)-width_ / 4.0f) );
            ImGui::SetCursorPosX((width_ * 0.75f) - (button_size_width_ / 2.0f));
            ImGui::SetCursorPosY(20);
            if (ImGui::Button("EDIT PROJECT", ImVec2(button_size_width_, button_size_height_)))
            {
                on_edit_project();
            }

            ImGui::PopStyleColor(1);

            export_time_popup();
            edit_project_popup();
            create_project_popup();
        }

        void ugv_path_gui::render_project_section()
        {

        }

        void ugv_path_gui::on_create_project()
        {
            if (create_project_ == true)
            {
                ImGui::OpenPopup("create_project_popup");
            }
            //create_project_popup();
        }

        void ugv_path_gui::on_edit_project()
        {
            ImGui::OpenPopup("edit_project_popup");
        }

        void ugv_path_gui::on_export_time()
        {
            ImGui::OpenPopup("export_time_popup");
        }

        void ugv_path_gui::export_time_popup()
        {
            if (ImGui::BeginPopup("export_time_popup"))
            {
                //export_times_implementation
            }
        }

        void ugv_path_gui::edit_project_popup()
        {
            if (ImGui::BeginPopup("edit_project_popup"))
            {
                //export_times_implementation
            }
        }

        void ugv_path_gui::create_project_popup()
        {
            if (ImGui::BeginPopup("create_project_popup"))
            {
                ImGui::Text("Project name");
                ImGui::SameLine();
                ImGui::InputText("", project_name_, PROJECT_NAME_MAX_LENGTH);

                if(ImGui::Button("Create"))
                {
                    create_project_ = false;

                    active_projects_.emplace_back(project(project_name_, 0, 0));
                    
                    auto found = std::find_if(active_projects_.begin(), active_projects_.end(),
                        [&project_name_ = this->project_name_](project& _prj) {return _prj.name_.c_str() == project_name_; });
                    
                    if (found == active_projects_.end())
                    {
                        gui_status_msgs_.push(GUI_STATUS::PROJECT_CREATE_SUCCESS);
                    }
                    else
                    {
                        gui_status_msgs_.push(GUI_STATUS::DUPLICATE_PROJECT_NAME);
                    }

                    std::memset(project_name_, '\0', PROJECT_NAME_MAX_LENGTH);

                    std::cout << "create button clicked" << '\n';
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();

            }
        }

        void ugv_path_gui::project_list_renderer()
        {
            const size_t max_columns = 3;//0-1
            const size_t max_rows = 10;

            const size_t row_offset = 100;
            const size_t column_offset = width_/(max_columns+1);

            size_t current_column = 1;
            size_t current_row = 1;

            for (const auto& project : active_projects_)
            {
                ImGui::SetCursorPos(ImVec2((current_column * column_offset), row_offset + (current_row * row_offset)));
                ImGui::Button(project.name_.c_str(),ImVec2(button_project_width_,button_project_height_));
                if (current_column == max_columns)
                {
                    current_row++;
                    current_column = 1;
                }
                else
                {
                    //ImGui::SameLine();
                    current_column++;
                }
            }
            
        }

        void ugv_path_gui::status_msg_popup()
        {
        }

        void ugv_path_gui::update()
        {
            ImGui::SetWindowSize("Project manager", ImVec2(width_, height_));

            ImGui::SetWindowPos("Project manager", ImVec2(0, 0));

            window_flags_ = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

            ImGuiStyleVar style = ImGuiStyleVar_FrameRounding;
            ImGui::PushStyleVar(style, 12);
            ImGui::Begin("Project manager",NULL, window_flags_);

            render_button_section();
            project_list_renderer();


            ImGui::End();

           // ImGui::ShowDemoWindow();
            ImGui::PopStyleVar(1);
        }

        void ugv_path_gui::new_frame()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void ugv_path_gui::shutdown()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            ImPlot::DestroyContext();
        }

        void ugv_path_gui::render_ui()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
    }
} //namespace lpp