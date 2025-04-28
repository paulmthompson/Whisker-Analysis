
include(FetchContent)

FetchContent_Declare(
  fast_float
  GIT_REPOSITORY https://github.com/fastfloat/fast_float.git
  GIT_TAG tags/v7.0.0
  GIT_SHALLOW TRUE)


FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.1
  GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(fast_float spdlog)


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()