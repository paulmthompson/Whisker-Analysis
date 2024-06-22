cmake_minimum_required(VERSION 3.24)

project(InSourceBuild)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#[[
Manifest mode from VCPKG will make all dependancies specified in vcpkg.json be associated and installed
with this particular project
]]
set(VCPKG_MANIFEST_MODE ON)

#[[
We are going to add ffmpeg_wrapper as a subdirectory and link to an executable that uses it

]]
set(BUILD_TESTING OFF CACHE INTERNAL "")
add_subdirectory(${Whisker-Analysis_source}
        ${CMAKE_CURRENT_BINARY_DIR}/Whisker-Analysis)

add_executable(InSourceBuild  EXCLUDE_FROM_ALL main.cpp)

target_link_libraries(InSourceBuild PRIVATE Whisker-Analysis)

