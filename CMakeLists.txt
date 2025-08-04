
cmake_minimum_required(VERSION 3.10)
project(BasicOpenGL)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Set custom executable output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $ENV{HOME}/CppProjectBuild)

# Include dirs
include_directories(
    include
    glad/include
    src
)

# Source files
file(GLOB SRC_FILES
    src/*.cpp
    glad/src/glad.c
)

# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})

# Executable
add_executable(BasicOpenGL ${SRC_FILES})

# Link libraries
target_link_libraries(BasicOpenGL ${GLFW_LIBRARIES} dl GL X11 pthread Xrandr Xi Xxf86vm Xcursor)

