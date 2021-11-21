add_executable(paramhelper_tests tests.cpp ParamHelper_t.cpp)
target_link_libraries(paramhelper_tests PUBLIC paramhelper Catch2::Catch2)

# allow user to run tests with `make test` or `ctest`
catch_discover_tests(tests)
