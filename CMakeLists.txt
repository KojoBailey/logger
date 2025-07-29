cmake_minimum_required(VERSION 3.19)
project(logger VERSION 0.1.0 DESCRIPTION "Kojo's Logging Library")
set(CMAKE_CXX_STANDARD 20)

add_library(logger INTERFACE)

target_include_directories(logger
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

add_library(kojo::logger ALIAS logger)

install(TARGETS logger
        EXPORT loggerTargets
        INCLUDES DESTINATION include)

install(EXPORT loggerTargets
        FILE loggerTargets.cmake
        NAMESPACE kojo::
        DESTINATION lib/cmake/logger)