
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_executable(test_parameterization test_constants.hpp test_distance.cpp test_set.cpp)

target_link_libraries(test_parameterization PRIVATE Catch2::Catch2WithMain Whisker-Analysis)

if (WIN32)
    catch_discover_tests(test_parameterization
            DL_PATHS
            "$ENV{PATH}"
            "${CMAKE_BINARY_DIR}"
    )
else()
    catch_discover_tests(test_parameterization)
endif()



