
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_executable(tests test_constants.hpp test_distance.cpp test_set.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain Whisker-Analysis)

if (WIN32)
    catch_discover_tests(tests
            DL_PATHS
            "$ENV{PATH}"
            "${CMAKE_BINARY_DIR}"
    )
else()
    catch_discover_tests(tests)
endif()



