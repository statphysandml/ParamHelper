include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v2.13.8
)
FetchContent_MakeAvailable(Catch2)

add_executable(paramhelper_tests tests.cpp ParamHelper_t.cpp)

target_include_directories(paramhelper_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${json_SOURCE_DIR}/include
)

target_link_libraries(paramhelper_tests PRIVATE
    paramhelper
    Catch2::Catch2
)

include(${catch2_SOURCE_DIR}/contrib/Catch.cmake)
catch_discover_tests(paramhelper_tests)