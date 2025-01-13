cmake_minimum_required (VERSION 3.8)

if(DEFINED PROJECT_NAME)
  set(subproject ON)
else()
  set(subproject OFF)
endif()

project(cpp_sugar VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(cpp_sugar INTERFACE)
add_library(ohtoai::cpp_sugar ALIAS cpp_sugar)
target_include_directories(cpp_sugar INTERFACE include)

if (NOT subproject)
  message(STATUS "Building cpp_sugar as standalone project")
  add_subdirectory (test)
endif()
