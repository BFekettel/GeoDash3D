cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)       # GLFW headers
include_directories(external)                           # GLM & GLAD headers

# Library directories
link_directories(${CMAKE_SOURCE_DIR}/lib)              # glfw3.lib location

# OpenGL
find_package(OpenGL REQUIRED)

# Add executable and source files
add_executable(Project
        main.cpp
        external/glad/src/glad.c
)

# Link libraries
target_link_libraries(Project
        glfw3
        ${OPENGL_gl_LIBRARY}
        gdi32
        user32
        shell32
)
