
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_executable(tests
        test_whiskertracker.cpp
        test_data.hpp
        test_loaders.cpp
)

target_link_libraries(tests PRIVATE
        Catch2::Catch2WithMain
        Whisker-Analysis
)

if(ENABLE_IWYU)
    set_target_properties(tests PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
endif()

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

set(DATA_FILES_TO_COPY
        "test_img.bin"
        "test_img2.bin"
        "test_img3.bin"
        "0110_1_0000000.bin"
        "whiskers.csv"
)

copy_files(tests
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "${CMAKE_CURRENT_BINARY_DIR}/data"
        "${DATA_FILES_TO_COPY}"
)

set(PYTEST_FILES_TO_COPY
        "test_whiskertracker.py"
        "conftest.py"
)

copy_files(tests
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${PYTEST_FILES_TO_COPY}"
)

if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND WIN32))
    add_test(NAME python-tests
            COMMAND ${Python3_EXECUTABLE} -m
            pytest ${CMAKE_BINARY_DIR}/tests/WhiskerTracking
            --dirpath=${CMAKE_CURRENT_BINARY_DIR}/data
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    set_property(TEST python-tests APPEND PROPERTY
            ENVIRONMENT
            "PATH=$<TARGET_FILE_DIR:Whisker-Analysis>;$ENV{PATH}"
    )
endif()
