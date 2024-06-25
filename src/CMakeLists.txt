
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

target_link_libraries(Whisker-Analysis PUBLIC janelia)

target_include_directories(Whisker-Analysis PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerTracking>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerParameterization>"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Whisker-Analysis PRIVATE ${CLANG_OPTIONS})
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