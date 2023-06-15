cmake_minimum_required(VERSION 3.26)

project(ophiuchus LANGUAGES CXX)

include("cmake/StandardSettings.cmake")
standard_settings()

find_package(Vulkan REQUIRED)

add_subdirectory(src)
