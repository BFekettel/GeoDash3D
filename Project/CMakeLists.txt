cmake_minimum_required(VERSION 3.16)
project(GeoDash3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Optional: pass -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

# === Sources ===
set(PROJECT_SOURCES
        src/main.cpp
        src/Entity/Entity.cpp
        src/Entity/LightEntity.cpp

        src/Managers/ManagerRegistry.h # headers not necessary but okay
        src/Managers/ManagerRegistry.cpp
        src/Managers/PhysicsManager.h
        src/Managers/PhysicsManager.cpp

        src/Components/Component.cpp
        src/Components/Component.h
        src/Components/PhysicsComp.h
        src/Components/ControllerComponent.cpp
        src/Components/RadiusLightComp.cpp
        src/Components/StaticMeshComp.cpp

        src/Rendering/Camera.cpp
        src/Rendering/RenderManager.cpp
        src/Rendering/Shader.cpp

        src/Developer/DevGui.cpp

        # ImGui (local)
        libs/imgui/imgui.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/backends/imgui_impl_glfw.cpp
        libs/imgui/backends/imgui_impl_opengl3.cpp
        src/Entity/LightEntity.cpp
        src/Managers/ManagerRegistry.cpp
        src/Managers/ManagerRegistry.h
        src/Managers/LevelManager.h
        src/Managers/PhysicsManager.cpp
        src/Managers/PhysicsManager.h
        src/Components/PhysicsComp.cpp
        src/Components/PhysicsComp.h
        src/Managers/IManager.h
        src/Physics/AABB.h
        tests/test_aabb.cpp
)

# === Glad as internal static library (compiles glad.c and exposes include) ===
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/external/glad/include)
set_target_properties(glad PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# === Include dirs ===
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/libs/imgui
        ${CMAKE_SOURCE_DIR}/libs/imgui/backends
)

set(RESOURCES
        ${CMAKE_SOURCE_DIR}/shaders/basic.vert
        ${CMAKE_SOURCE_DIR}/shaders/basic.frag
        ${CMAKE_SOURCE_DIR}/content/models/cube.obj
        ${CMAKE_SOURCE_DIR}/content/models/monkey.obj
        # add any other files you need
)

set(RESOURCE_DIR "${CMAKE_BINARY_DIR}/resources")
file(MAKE_DIRECTORY ${RESOURCE_DIR})

foreach(_res IN LISTS RESOURCES)
    get_filename_component(_resname ${_res} NAME)
    add_custom_command(
            TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${_res} ${RESOURCE_DIR}/${_resname}
    )
endforeach()


# GoogleTest unit tests
find_package(GTest CONFIG QUIET)
if (GTest_FOUND)
    enable_testing()
    add_executable(geodash3d_tests
            tests/test_aabb.cpp
    )
    target_include_directories(geodash3d_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_link_libraries(geodash3d_tests PRIVATE GTest::gtest_main glm::glm)
    add_test(NAME aabb_tests COMMAND geodash3d_tests)
else()
    message(WARNING "GoogleTest not found. Install via vcpkg (vcpkg install gtest:x64-linux) or disable tests.")
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Prefer vcpkg config-mode for glfw; fallback to pkg-config
find_package(glfw3 CONFIG QUIET)
if (glfw3_FOUND)
    if (TARGET glfw3)
        target_link_libraries(${PROJECT_NAME} PRIVATE glfw3)
    elseif (TARGET glfw)
        target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
    endif()
else()
    find_package(PkgConfig QUIET)
    if (PKG_CONFIG_FOUND)
        pkg_check_modules(GLFW_PKG REQUIRED glfw3)
        target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_PKG_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_PKG_LIBRARIES})
        target_compile_options(${PROJECT_NAME} PRIVATE ${GLFW_PKG_CFLAGS_OTHER})
    else()
        message(WARNING "GLFW not found; please install glfw3 or use vcpkg")
    endif()
endif()

# Link dependent libraries
find_package(glm CONFIG QUIET)
if (glm_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
endif()

find_package(assimp CONFIG QUIET)
if (assimp_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
endif()

find_package(nlohmann_json CONFIG QUIET)
if (nlohmann_json_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
endif()

find_package(OpenAL CONFIG QUIET)
if (OpenAL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenAL::OpenAL)
endif()

# Link glad (internal)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# Link OpenGL
if (TARGET OpenGL::GL)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
endif()

# Platform-specific libraries
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE gdi32 user32 shell32)
else()
    # linux needs dl for runtime symbol loading
    find_library(DL_LIB dl)
    if (DL_LIB)
        target_link_libraries(${PROJECT_NAME} PRIVATE ${DL_LIB})
    endif()
endif()

# Warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

# Expose header files to IDE
target_sources(${PROJECT_NAME} PRIVATE src/Rendering/Shader.h src/Developer/DevGui.h)
