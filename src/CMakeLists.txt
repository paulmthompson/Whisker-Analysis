
set(stageDir ${CMAKE_BINARY_DIR})
include(GNUInstallDirs)
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${stageDir})
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${stageDir})
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${stageDir})
endif()

#find_package(Python3 COMPONENTS Interpreter Development)
find_package(OpenMP REQUIRED)
if(MSVC)
    set(OpenMP_CXX_FLAGS "-openmp:experimental")
endif()
message(STATUS "OpenMP_CXX_FLAGS: ${OpenMP_CXX_FLAGS}")

find_package(Armadillo REQUIRED)

add_subdirectory(WhiskerTracking/JaneliaWhiskerTracker)

add_library(Whisker-Analysis SHARED
        WhiskerTracking/whiskertracker.cpp
        WhiskerTracking/whiskertracker.hpp

        WhiskerParameterization/whisker.hpp
        WhiskerParameterization/whisker.cpp
        WhiskerParameterization/geometry.hpp
        WhiskerParameterization/Geometry/points.hpp
        WhiskerParameterization/Geometry/points.cpp
        WhiskerParameterization/Geometry/lines.hpp
        WhiskerParameterization/Geometry/lines.cpp
        WhiskerParameterization/Geometry/vector.hpp
        WhiskerParameterization/Geometry/vector.cpp
        WhiskerParameterization/Geometry/mask.hpp
        WhiskerParameterization/Geometry/curvature.hpp
        WhiskerParameterization/Geometry/curvature.cpp

        WhiskerTracking/loaders.hpp
        WhiskerTracking/loaders.cpp

)

add_library(Whisker-Analysis::Whisker-Analysis ALIAS Whisker-Analysis)

target_link_libraries(Whisker-Analysis PUBLIC janelia OpenMP::OpenMP_CXX)

target_link_libraries(Whisker-Analysis PUBLIC fast_float)

target_link_libraries(Whisker-Analysis PUBLIC spdlog::spdlog)

target_link_libraries(Whisker-Analysis PRIVATE armadillo )

target_include_directories(Whisker-Analysis PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerTracking>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerParameterization>"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Whisker-Analysis PRIVATE ${CLANG_OPTIONS})
    #set_target_properties(Whisker-Analysis PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Whisker-Analysis PRIVATE ${GCC_WARNINGS})
    #set_target_properties(Whisker-Analysis PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if (MSVC)
    target_compile_options(Whisker-Analysis PRIVATE ${MSVC_WARNINGS})
endif()

if(BUILD_WHISKERTRACKER)

    set(PYBIND11_FINDPYTHON ON) # Ensure pybind11 finds Python

    FetchContent_Declare(
            pybind11
            GIT_REPOSITORY https://github.com/pybind/pybind11
            GIT_TAG v2.12.0
    )

    FetchContent_GetProperties(pybind11)
    if(NOT pybind11_POPULATED)
        FetchContent_Populate(pybind11)
        add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
    endif()

    pybind11_add_module(whisker MODULE 
        Python/WhiskerTracking/whiskertracker_pybind.cpp
        Python/python_main.cpp
        Python/utils.hpp)
    
    if(ENABLE_IWYU)
        set_target_properties(whisker PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
    endif()
    
    target_link_libraries(whisker PRIVATE Whisker-Analysis pybind11::module)

    if (WIN32)
        set_target_properties(whisker PROPERTIES SUFFIX ".pyd")
    elseif(APPLE)
        set_target_properties(whisker PROPERTIES SUFFIX ".so") #I think?
    else()
        set_target_properties(whisker PROPERTIES SUFFIX ".so")
    endif()
endif()

install(TARGETS Whisker-Analysis
        BUNDLE DESTINATION .
        LIBRARY
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)