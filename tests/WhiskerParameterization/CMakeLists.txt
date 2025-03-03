
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

add_executable(test_parameterization 
    test_constants.hpp 
    test_set.cpp
    Geometry/test_vector.cpp
    Geometry/test_mask.cpp 
    Geometry/test_points.cpp
    Geometry/test_lines.cpp 
    Geometry/test_curvature.cpp
    )

if(ENABLE_IWYU)
    set_target_properties(test_parameterization PROPERTIES CXX_INCLUDE_WHAT_YOU_USE "")
endif()

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
