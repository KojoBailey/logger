cmake_minimum_required(VERSION 3.19)

project(kojo-log-lib VERSION 0.1.0 DESCRIPTION "Kojo's Logging Library")

# Find source files
# file(GLOB SOURCES src/*.cpp)

# # Create shared library
# add_library(kojo-log-lib STATIC ${SOURCES})
add_library(kojo-log-lib INTERFACE)

# Include header files
target_include_directories(kojo-log-lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(kojo-log-lib PROPERTIES CXX_STANDARD 23)