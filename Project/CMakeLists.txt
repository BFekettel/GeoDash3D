cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)       # GLFW headers
include_directories(external)                           # GLM & GLAD headers
include_directories(${CMAKE_SOURCE_DIR}/src)          # all headers in src
include_directories(external)                          # GLM & GLAD

include_directories(
        ${CMAKE_SOURCE_DIR}/libs/imgui
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)

# Library directories
link_directories(${CMAKE_SOURCE_DIR}/lib)              # glfw3.lib location

# OpenGL
find_package(OpenGL REQUIRED)

# Add executable and source files
add_executable(Project
        src/main.cpp
        external/glad/src/glad.c
        src/Entity/Entity.cpp
        src/Components/Component.cpp
        src/Components/StaticMeshComp.cpp
        src/Components/ControllerComponent.cpp
        src/Rendering/Camera.cpp
        src/Rendering/RenderManager.cpp
        src/Rendering/Shader.cpp
        src/Developer/DevGui.cpp
        # ImGui sources
        libs/imgui/imgui.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
)



# Link libraries
target_link_libraries(Project
        glfw3
        assimp-vc143-mt
        ${OPENGL_gl_LIBRARY}
        gdi32
        user32
        shell32
)
