
add_subdirectory(in-source-build)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.5.4 # or a later release
)

FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(Catch)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

message(STATUS "${Python3_RUNTIME_LIBRARY_DIRS}")

add_subdirectory(WhiskerParameterization)

add_subdirectory(WhiskerTracking)




add_executable(benchmark
    WhiskerParameterization/benchmark_set.cpp
    WhiskerParameterization/Geometry/benchmark_lines.cpp
    WhiskerParameterization/Geometry/benchmark_points.cpp
    WhiskerTracking/benchmark_loaders.cpp
    WhiskerTracking/benchmark_whiskertracker.cpp
)

target_link_libraries(benchmark PRIVATE Catch2::Catch2WithMain Whisker-Analysis)

if (WIN32)
    catch_discover_tests(benchmark
            DL_PATHS
            "$ENV{PATH}"
            "${CMAKE_BINARY_DIR}"
    )
else()
    catch_discover_tests(benchmark)
endif()