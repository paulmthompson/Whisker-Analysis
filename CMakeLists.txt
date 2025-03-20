#=============================
# Preamble
#=============================
cmake_minimum_required(VERSION 3.25)
project(Whisker-Analysis VERSION 0.1 LANGUAGES CXX)

include(CTest)

#=============================
# Project Wide Setup
#=============================

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#[[
Best practice for looking for cmake modules cmake folder in root of the project
Currently, this project does not have any cmake modules
From Professional CMake Chapter 11 Modules
]]
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(enable_sanitizers)
include(CompilerWarnings)
include(set_rpath)
include(iwyu)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    option(CODE_COVERAGE "Enable coverage reporting" OFF)
    if(CODE_COVERAGE)
        add_compile_options(--coverage -O2 -g -fprofile-update=atomic)
        add_link_options(--coverage -fprofile-update=atomic)
    endif()
endif()

#[[
Manifest mode from VCPKG will make all dependencies specified in vcpkg.json be associated and installed
with this particular project
]]
set(VCPKG_MANIFEST_MODE ON)

#This is necessary to work on windows. A better way would be to define only the symbols that need to be exported
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_SUPPORT_WINDOWS_EXPORT_ALL_SYMBOLS 1)

option(BUILD_WHISKERTRACKER "Build the whiskertracker target" OFF)

if (PROJECT_IS_TOP_LEVEL)
    set(BUILD_WHISKERTRACKER ON)
endif()

# Define the output directory for Python files
set(PYTHON_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Copy the Python files to the output directory
configure_file(
    ${PROJECT_SOURCE_DIR}/python/__init__.py 
    ${PYTHON_OUTPUT_DIRECTORY}/__init__.py COPYONLY)
configure_file(
    ${PROJECT_SOURCE_DIR}/python/whisker_analysis.py
    ${PYTHON_OUTPUT_DIRECTORY}/whisker_analysis.py COPYONLY)


#=============================
# Externally Provided Content
#=============================

add_subdirectory(external)

#=============================
# Main Build Targets
#=============================

add_subdirectory(src)


#=============================
# Testing
#=============================

#[[
BUILD_TESTING is a cache variable that defaults to true which was defined by the CTest module
If we wish to run testing, we should add the tests subdirectory.
]]
if (BUILD_TESTING)

    if (ENABLE_IWYU)
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "") #Disable for tests
    endif()

    add_subdirectory(tests)
endif()