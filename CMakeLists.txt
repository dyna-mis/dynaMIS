cmake_minimum_required(VERSION 3.15)
project(dynaMIS)
add_subdirectory(src)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT dynaMIS)

