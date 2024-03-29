cmake_minimum_required(VERSION 3.0.0)
project(project_manager VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(CTest)
enable_testing()

include_directories(include)

SET(HEADERS include/project_manager_gui.hpp include/common.hpp include/project.hpp include/project_manager.hpp include/utilities.hpp include/save_file_manager.hpp)

SET(SOURCES src/main.cpp src/project_manager_gui.cpp src/glad.c src/project_manager.cpp)


find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(project_manager ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} PRIVATE glfw )
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE implot::implot ${RABBIT_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
