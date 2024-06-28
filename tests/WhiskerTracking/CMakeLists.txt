
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

function(copy_files TARGET_NAME SOURCE_DIR DEST_DIR FILES)
    foreach(FILE ${FILES})
        add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                ${SOURCE_DIR}/${FILE}
                ${DEST_DIR}/${FILE})
    endforeach()
endfunction()

set(FILES_TO_COPY "test_img.bin" "test_img2.bin")
copy_files(tests
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "${CMAKE_CURRENT_BINARY_DIR}/data"
        "${FILES_TO_COPY}"
)