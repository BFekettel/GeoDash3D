cmake_minimum_required(VERSION 3.16)
project(Project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GLFW headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Library folder with glfw3.lib
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(Project main.cpp)

# OpenGL
find_package(OpenGL REQUIRED)

# Link GLFW + dependencies
target_link_libraries(Project
        glfw3
        ${OPENGL_gl_LIBRARY}
        gdi32
        user32
        shell32
)
