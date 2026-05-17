cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# -----------------------------
# Assimp (stable release)
# -----------------------------
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG v5.3.1
)

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

FetchContent_MakeAvailable(assimp)

# -----------------------------
# ImGui
# -----------------------------
set(IMGUI_SRC
        libs/imgui/imgui.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui ${IMGUI_SRC})
target_include_directories(imgui PUBLIC libs/imgui libs/imgui/backends)

# -----------------------------
# GLAD
# -----------------------------
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# -----------------------------
# OpenGL + GLFW
# -----------------------------
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# -----------------------------
# Executable
# -----------------------------
add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Entity/Entity.cpp
        src/Components/Component.cpp
        src/Components/StaticMeshComp.cpp
        src/Components/ControllerComponent.cpp
        src/Rendering/Camera.cpp
        src/Rendering/RenderManager.cpp
        src/Rendering/Shader.cpp
        src/Developer/DevGui.cpp
        src/Components/LightComp.cpp
        src/Entity/LightEntity.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        external
        external/glad/include
        external/stb
        src
)

# -----------------------------
# Linking
# -----------------------------
target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glfw
        glad
        assimp
        imgui
        OpenGL::GL
)
