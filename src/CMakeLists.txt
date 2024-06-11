
add_subdirectory(WhiskerTracking/JaneliaWhiskerTracker)

add_library(Whisker-Analysis SHARED
        WhiskerTracking/whiskertracker.cpp
        WhiskerTracking/whiskertracker.hpp
)

add_library(Whisker-Analysis::Whisker-Analysis ALIAS Whisker-Analysis)

target_link_libraries(Whisker-Analysis PRIVATE janelia)

target_include_directories(Whisker-Analysis PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/WhiskerTracking>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_INSTALL_BINDIR})
endif()