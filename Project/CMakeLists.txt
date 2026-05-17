cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Gets the assimp dll for the project.

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Missing description." FORCE)
FetchContent_Declare(assimp
GIT_REPOSITORY https://github.com/assimp/assimp.git
GIT_TAG master)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(assimp)



# Include directories
include_directories(external)                           # GLM & GLAD headers
include_directories(external/glad/include)                           # GLM & GLAD headers
include_directories(${CMAKE_SOURCE_DIR}/src)          # all headers in src
include_directories(external)                          # GLM & GLAD

include_directories(external/stb)                          # stb

include_directories(
        ${CMAKE_SOURCE_DIR}/libs/imgui
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)

# OpenGL
find_package(OpenGL REQUIRED)

add_library(glad external/glad/src/glad.c)

# Add executable and source files
add_executable(${PROJECT_NAME}
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
        src/Components/LightComp.cpp
        src/Components/LightComp.h
        src/Entity/LightEntity.cpp
        src/Entity/LightEntity.h
)

target_include_directories(${PROJECT_NAME} PRIVATE libs/assimp-src/include)

find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)

# Link libraries
target_link_libraries(${PROJECT_NAME}
        glfw
        assimp
        OpenGL::GL
)
