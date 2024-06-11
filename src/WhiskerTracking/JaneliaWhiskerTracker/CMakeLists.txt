
add_library(janelia SHARED
        janelia.hpp
        janelia.cpp
        detector_bank.hpp
        detector_bank.cpp
        io.hpp)

target_include_directories(janelia
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    )


