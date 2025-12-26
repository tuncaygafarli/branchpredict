cmake_minimum_required(VERSION 3.16)
project(ImGuiApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_library(imgui STATIC
    vendor/ImGui/imgui.cpp
    vendor/ImGui/imgui_draw.cpp
    vendor/ImGui/imgui_tables.cpp
    vendor/ImGui/imgui_widgets.cpp
    vendor/ImGui/imgui_demo.cpp
    vendor/ImGui/imgui_impl_glfw.cpp
    vendor/ImGui/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    vendor/ImGui
)

target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL
)

add_library(core STATIC
    cpu/cpu.cpp
    cpu/cpu.h
    cpu/branchpredictor.cpp
    cpu/branchpredictor.h
    cpu/instruction.cpp
    cpu/instruction.h

    gui/gui_elements.cpp
    gui/gui_elements.h
    gui/gui_renderer.cpp
    gui/gui_renderer.h

    parser/lookup.cpp
    parser/lookup.h
    parser/parser.cpp
    parser/parser.h
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core PUBLIC imgui)

add_executable(cpu_visual
    main.cpp
)

target_link_libraries(cpu_visual PRIVATE core)