
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_executable(tests test_whiskertracker.cpp test_data.hpp)

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

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)

add_custom_command(TARGET tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/data/test_img.bin
        ${CMAKE_CURRENT_BINARY_DIR}/data/test_img.bin)