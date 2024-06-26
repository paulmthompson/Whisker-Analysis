
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

add_subdirectory(WhiskerTracking/JaneliaWhiskerTracker)

add_library(Whisker-Analysis SHARED
        WhiskerTracking/whiskertracker.cpp
        WhiskerTracking/whiskertracker.hpp

        WhiskerParameterization/whisker.hpp
        WhiskerParameterization/whisker.cpp
        WhiskerParameterization/geometry.hpp
        WhiskerParameterization/Geometry/points.hpp
        WhiskerParameterization/Geometry/lines.hpp
        WhiskerParameterization/Geometry/vector.hpp
        WhiskerParameterization/Geometry/mask.hpp

)

add_library(Whisker-Analysis::Whisker-Analysis ALIAS Whisker-Analysis)

pybind11_add_module(whiskertracker MODULE WhiskerTracking/whiskertracker_pybind.cpp)

set_target_properties(whiskertracker PROPERTIES OUTPUT_NAME "whiskertracker")

target_link_libraries(Whisker-Analysis PUBLIC janelia)

target_link_libraries(whiskertracker PRIVATE Whisker-Analysis)
target_link_libraries(whiskertracker PRIVATE pybind11::module)

target_include_directories(Whisker-Analysis PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerTracking>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerParameterization>"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Whisker-Analysis PRIVATE ${CLANG_OPTIONS})
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(Whisker-Analysis PRIVATE ${GCC_WARNINGS})
endif()

if (MSVC)
    target_compile_options(Whisker-Analysis PRIVATE ${MSVC_WARNINGS})
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