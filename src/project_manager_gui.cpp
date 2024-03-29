#include "project_manager_gui.hpp"
#include <chrono>
#include <functional>
#include <imgui.h>
#include <implot.h>
#include <memory>
#include <thread>
#include <iostream>
#include "save_file_manager.hpp"

namespace pm
{
    namespace gui
    {
        void project_manager_gui::init(GLFWwindow *_window, const char *_glsl_version,const int& _width, const int& _height)
        {
            width_ = _width;
            height_ = _height;

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImPlot::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            //io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
            //io.Fonts->AddFontFromFileTTF("fonts/active_font.ttf", 15);
            //io.Fonts->AddFontFromFileTTF("C:\\dev\\repos\\project_manager\\build\\Release\\fonts\\OpenSans-Bold.ttf", 18);
            io.Fonts->AddFontFromFileTTF("fonts\\OpenSans-Bold.ttf", 18);

            //io.DeltaTime = 10.0f;
           // std::thread path_controller_thread();

            ImGui_ImplGlfw_InitForOpenGL(_window, true);
            ImGui_ImplOpenGL3_Init(_glsl_version);
            ImGui::StyleColorsDark();
            //ImGuiStyleVar_FrameRounding;
        }


        void project_manager_gui::render_button_section()
        {
            //ImGui::SetCursorPos(ImVec2(20, 20));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.8f, 0.8f, 0.3f));

            //ImGui::SetCursorPosX(((width_)-width_/1.33333f) - (button_size_width_));
            ImGui::SetCursorPosX((width_*0.25f) - (button_size_width_/1.33333f));
            ImGui::SetCursorPosY(20);
            if (ImGui::Button("CREATE PROJECT", ImVec2(button_size_width_, button_size_height_)))
            {
                create_project_ = true;
                on_create_project();
            }
            
            //ImGui::SetCursorPosX(((width_)-width_ / 2.0f) - (button_size_width_ / 2.0f));
            ImGui::SetCursorPosX((width_ * 0.5f) - (button_size_width_ / 2.0f));
            ImGui::SetCursorPosY(20);
            if (ImGui::Button("EXPORT/LOAD\n        TIMES", ImVec2(button_size_width_, button_size_height_)))
            {
                on_export_time();
            }

            //ImGui::SetCursorPosX(((width_)-width_ / 4.0f) );
            ImGui::SetCursorPosX((width_ * 0.75f) - (button_size_width_ / 4.0f));
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

        void project_manager_gui::render_project_section()
        {

        }

        void project_manager_gui::on_create_project()
        {
            if (create_project_ == true)
            {
                ImGui::OpenPopup("create_project_popup");

                
            }
            //create_project_popup();
        }

        void project_manager_gui::on_edit_project()
        {
            ImGui::OpenPopup("edit_project_popup");
        }

        void project_manager_gui::on_export_time()
        {
            ImGui::OpenPopup("export_time_popup");
        }

        void project_manager_gui::export_time_popup()
        {
            if (ImGui::BeginPopup("export_time_popup"))
            {
                if (ImGui::Button("Save"))
                {
                    auto project_json = pm::save_file::vector_to_json(project_manager_.get_projects());
                    pm::save_file::save_json_to_file(project_json);
                }
                if (ImGui::Button("Save & Export"))
                {
                    auto project_json = pm::save_file::vector_to_json(project_manager_.get_projects());
                    pm::save_file::save_json_to_file(project_json);
                    pm::save_file::export_to_csv(project_json);
                }
                if (ImGui::Button("Load"))
                {
                    auto js = pm::save_file::read_config("config_js.json");
                    if (js.has_value())
                    {
                        auto loaded_projects = pm::save_file::json_to_container<project>(js.value());
                        project_manager_.set_loaded_projects(loaded_projects);
                    }
                }
                //export_times_implementation
                ImGui::EndPopup();
            }
        }

        void project_manager_gui::edit_project_popup()
        {
            ImGui::SetNextWindowPos(ImVec2(30, 80));
            if (ImGui::BeginPopup("edit_project_popup"))
            {
                ImGui::Spacing();
                ImGui::BeginTable("projects edit", 6, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings| ImGuiTableFlags_Borders);
                
                ImGui::TableNextColumn();
                ImGui::Text("Project");

                ImGui::TableNextColumn();
                ImGui::Text("Date Created");

                ImGui::TableNextColumn();
                ImGui::Text("Time");

                ImGui::TableNextColumn();
                ImGui::Text("Color");

                ImGui::TableNextColumn();
                ImGui::Text("Description");

                ImGui::TableNextColumn();
                ImGui::Text("Del");
                
               
                for (size_t i = 0; const auto & prj : project_manager_.get_projects())
                {
                    ImGui::TableNextRow();   //project name
                    ImGui::TableNextColumn();
                    //ImGui::Text(prj.name_.c_str());
                    if (ImGui::CalcTextSize(prj.name_.c_str()).x > 8)
                    {
                        std::string first_str = prj.name_.substr(0, 4);
                        std::string text_str = first_str + "...";
                        ImGui::Text(text_str.c_str());
                        if (ImGui::IsItemHovered())
                        {
                            ImGui::BeginTooltip();
                            ImGui::Text(prj.name_.c_str());
                            ImGui::EndTooltip();
                        }
                    }
                    else
                    {
                        ImGui::Text(prj.description_.c_str());
                    }

                    ImGui::TableNextColumn();//date created
                    std::time_t date_created = std::chrono::system_clock::to_time_t(prj.time_created);
                    ImGui::Text(std::ctime(&date_created));

                    ImGui::TableNextColumn();//time
                    ImGui::Text(project_manager_.project_time_to_string(prj.overall_time_).c_str());

                    ImGuiStyleVar style = ImGuiStyleVar_FrameRounding;
                    ImGui::PushStyleVar(style, 0);

                    ImGui::PushStyleColor(ImGuiCol_Button, prj.color_);
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, prj.color_);
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, prj.color_);

                    ImGui::TableNextColumn();//color
                    ImGui::Button(" ", ImVec2(35, 20));
                    ImGui::PopStyleVar();
                    ImGui::PopStyleColor(3);

                    ImGui::TableNextColumn();//description
                    if (ImGui::CalcTextSize(prj.name_.c_str()).x > 11)
                    {
                        std::string first_str = prj.description_.substr(0, 8);
                        std::string text_str = first_str + "...";
                        ImGui::Text(text_str.c_str());
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        if (prj.description_.size() <= PROJECT_DESC_MAX_LENGTH / 3)
                        {
                            ImGui::Text(prj.description_.c_str());
                        }
                        else if (prj.description_.size() <= 2 * (PROJECT_DESC_MAX_LENGTH / 3))
                        {
                            ImGui::Text(format_desc_string(prj.description_,2).c_str());
                        }
                        else
                        {
                            ImGui::Text(format_desc_string(prj.description_, 3).c_str());
                        }
                        
                        ImGui::EndTooltip();
                    }

                    ImGui::TableNextColumn();//delete
                    std::string button_label = "X##" + std::to_string(i);
                    if (ImGui::Button(button_label.c_str(), ImVec2(25, 20)))
                    {
                        project_manager_.delete_project(i);
                    }
                    i++;
                }
                ImGui::EndTable();
                //export_times_implementation
                ImGui::EndPopup();
            }
        }
        void project_manager_gui::on_create_project_button()
        {
            create_project_ = false;


            auto found_iter = std::find_if(project_manager_.get_projects().begin(), project_manager_.get_projects().end(),
                [&project_name_ = this->project_name_](project& _prj) {return std::string(_prj.name_.c_str()) == std::string(project_name_); });

            if (found_iter == project_manager_.get_projects().end() && project_name_[0] != '\0')
            {
                //project_manager_.get_projects().emplace_back(project(project_name_, std::chrono::milliseconds(0), std::chrono::milliseconds(0)));
                project_manager_.add_project(project_name_,current_button_color_,project_desc_);
                gui_status_msgs_.push(GUI_STATUS::PROJECT_CREATE_SUCCESS);
            }
            else
            {
                gui_status_msgs_.push(GUI_STATUS::DUPLICATE_PROJECT_NAME);
            }

            std::memset(project_name_, '\0', PROJECT_NAME_MAX_LENGTH);
            std::memset(project_desc_, '\0', PROJECT_DESC_MAX_LENGTH);

            //std::cout << "create button clicked" << '\n';
            ImGui::CloseCurrentPopup();
        }

        void project_manager_gui::create_project_popup()
        {
            if (ImGui::BeginPopup("create_project_popup"))
            {
                ImGui::Text("Project name");
                ImGui::SameLine();
                ImGui::InputText("##", project_name_, PROJECT_NAME_MAX_LENGTH);
                show_color_picker();
                ImGui::InputTextMultiline("###", project_desc_, PROJECT_DESC_MAX_LENGTH, ImVec2(300, 100));
                //ImGui::InputText("###", project_desc_, PROJECT_DESC_MAX_LENGTH);
                if(ImGui::Button("Create"))
                {
                    on_create_project_button();
                }

                ImGui::EndPopup();
            }
            ImGui::SameLine();
        }

        void project_manager_gui::project_list_renderer()
        {
            const size_t max_columns = 2;//0-1
            const size_t max_rows = 10;

            const size_t row_offset = 100;
            const size_t column_offset = width_/(max_columns+1);
            float substract_var = 2.0;

            size_t current_column = 0;
            size_t current_row = 0;

            std::string project_text = "";
            auto sys_time_now = std::chrono::system_clock::now();

           // auto worktime = std::format(project_manager_gui::time_format, sys_time_now.time_since_epoch());

            for (size_t i = 0;const auto& prj : project_manager_.get_projects())
            {
                auto col_hovered = prj.color_;
                col_hovered.w = 0.5;
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, col_hovered);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, prj.color_);

                if (project_manager_.get_active_project_id() == i)
                {
                    //ImGui::PushTextWrapPos(5.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.1f, 0.1f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);
                    ImGui::PushStyleColor(ImGuiCol_Button, prj.color_);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(IM_COL32_WHITE));
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
                }
                else
                {
                    
                    //ImGui::PushTextWrapPos(5.0f);
                    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.1f, 0.1f));
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
                    ImGui::PushStyleColor(ImGuiCol_Button, prj.color_);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(IM_COL32_BLACK));
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.2f, 0.2f, 0.2f, 4.0f));
                }
                
                std::string project_name = prj.name_;
                project_text = project_manager_.get_project_string(i);

                ImGui::SetCursorPos(ImVec2(static_cast<float>(column_offset + (current_column * column_offset) - (button_project_width_*1.2f)),static_cast<float>( row_offset + (current_row * row_offset))));
                ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 100.0f);
                if (ImGui::Button(project_text.c_str(), ImVec2(button_project_width_, button_project_height_)))
                {
                    on_project_button_active(project_name);
                }
                substract_var *= 2.0f;
                if (current_column == max_columns)
                {
                    current_row++;
                    current_column = 0;
                }
                else
                {
                    //ImGui::SameLine();
                    current_column++;
                }
                ImGui::PopStyleColor(5);
                ImGui::PopStyleVar(2);
                ImGui::PopTextWrapPos();
                i++;
            }
            
        }

        void project_manager_gui::on_project_button_active(const std::string_view& _project_name)
        {
            auto iter = std::find_if(project_manager_.get_projects().begin(), project_manager_.get_projects().end(),
                [&_project_name](project& _prj) { return _prj.name_ == _project_name; });
           
            std::size_t index = std::distance(std::begin(project_manager_.get_projects()), iter);
            //std::cout << "trying to set active project:" << index <<'\n';
            project_manager_.set_active_project(index);
        }

        void project_manager_gui::status_msg_popup()
        {
            auto time = std::chrono::high_resolution_clock::now();
            if (!gui_status_msgs_.empty())
            {
                popup_start_time_ = std::chrono::duration_cast<std::chrono::microseconds>(time.time_since_epoch());
                popup_decay_time_ = popup_start_time_;

                auto status_msg = gui_status_msgs_.front();
                gui_status_msgs_.pop();

                switch (status_msg)
                {
                case GUI_STATUS::DUPLICATE_PROJECT_NAME:
                    popup_text_color_ = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                    popup_status_string_ = "Cannot create project - DUPLICATE PROJECT NAME";
                    //std::cout << "DUPLICATE " << '\n';
                    break;
                case GUI_STATUS::PROJECT_CREATE_SUCCESS:
                    popup_text_color_ = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                    popup_status_string_ = "Project created succesfully";
                    break;
                default:
                    break;
                }

                popup_tp_ = std::chrono::high_resolution_clock::now();
            }
            else
            {
                //popup_decay_time_ = std::time_t{ 0 };
            }
        }

        void project_manager_gui::render_popup_message()
        {
            ImGui::SetCursorPos(ImVec2(0, height_ - 22.0f));
            ImGui::BeginChild("status-popup", ImVec2(static_cast<float>(width_), 22.0f),true,window_flags_);
            ImGui::SetCursorPos(ImVec2(5.0f, 0.0f));
            auto time_now = std::chrono::high_resolution_clock::now();

            auto time_difference = std::chrono::duration_cast<std::chrono::seconds>(time_now - popup_tp_);

            //std::cout << time_difference.count() << '\n';

            if (time_difference.count() < 5)
            {
                popup_text_color_.w = 1.0f-(static_cast<float>(time_difference.count())/5.0f);
            }
            else
            {
                popup_text_color_.w = 0.0f;
            }
            

            ImGui::TextColored(popup_text_color_, popup_status_string_.c_str());

           ImGui::EndChild();
        }

        void project_manager_gui::update()
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Set 
            ImGui::SetWindowSize("Project manager", ImVec2(static_cast<float>(width_), static_cast<float>(height_)));

            ImGui::SetWindowPos("Project manager", ImVec2(0, 0));

            window_flags_ = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

            ImGuiStyleVar style = ImGuiStyleVar_FrameRounding;
            ImGui::PushStyleVar(style, 12);
            ImGui::Begin("Project manager",NULL, window_flags_);

            render_button_section();
            project_list_renderer();

            project_manager_.update_time();

            status_msg_popup();
            
            
            render_popup_message();

           // ImGui::ShowDemoWindow();
            ImGui::PopStyleVar(1);
            ImGui::PopStyleColor();
            ImGui::End();
        }

        void project_manager_gui::new_frame()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void project_manager_gui::shutdown()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
            ImPlot::DestroyContext();
        }

        void project_manager_gui::render_ui()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            //std::this_thread::sleep_for(std::chrono::duration<float>(0.033f));
        }

        void project_manager_gui::show_color_picker() {
            //static ImVec4 color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImGui::ColorPicker3("Color", reinterpret_cast<float*>(& current_button_color_));
        }

        std::string project_manager_gui::format_desc_string(std::string _desc,const unsigned _parts)
        {
            const char space = ' ';

            auto pos = _desc.find(space);
            std::vector<size_t> spaces_positions;

            if (pos == std::string::npos)
            {
                return format_no_space_string(_desc, _parts);
            }
            else
            {
                while (pos != std::string::npos)
                {
                    spaces_positions.emplace_back(pos);
                    pos = _desc.find(space, pos + 1);
                }

                unsigned addition = spaces_positions.size() / _parts; // 7 space 2 size => addition = 3

                if (addition == 0)
                {
                    std::cout << "only one space found" << '\n';
                    return format_no_space_string(_desc, _parts);
                }

                std::string formatted_text = _desc.substr(0, spaces_positions[addition]);
                for (int i = addition; i < spaces_positions.size()-addition; i = i + addition) // for (int i = 3 ; i < 7; i + addition) 3 6
                {
                    std::cout << i << '\n';
                    formatted_text += "\n" + _desc.substr(spaces_positions[i], (spaces_positions[addition+i] - spaces_positions[addition]));
                }

                return formatted_text;
            }
        }
        std::string project_manager_gui::format_no_space_string(std::string _desc, const unsigned _parts)
        {
            auto formatted_text = _desc.substr(0, PROJECT_DESC_MAX_LENGTH / 3);
            for (int i = _parts - 1; i > 0; i--)
            {
                formatted_text += + "\n" + _desc.substr(PROJECT_DESC_MAX_LENGTH / (i + 1), PROJECT_DESC_MAX_LENGTH / i);
            }
            std::cout << formatted_text << '\n';
            return formatted_text;
        }
    }
} //namespace lpp