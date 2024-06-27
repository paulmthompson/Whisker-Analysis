
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

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(janelia PRIVATE ${CLANG_OPTIONS})
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(janelia PRIVATE ${GCC_WARNINGS})
endif()

if (MSVC)
    target_compile_options(janelia PRIVATE ${MSVC_WARNINGS})
endif()

