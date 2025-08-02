cmake_minimum_required(VERSION 3.18)

# Set a name and a version number for your project:
project(paramhelper_examples VERSION 0.0.1 LANGUAGES CXX)

# Set C++ and CUDA standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include libraries
find_package(paramhelper CONFIG REQUIRED)

add_executable(paramhelper_examples param_helper_examples.cpp)
target_link_libraries(paramhelper_examples PUBLIC paramhelper::paramhelper)

# Print a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)