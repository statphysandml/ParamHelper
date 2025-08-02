cmake_minimum_required(VERSION 3.18)

# Set a name and a version number for your project:
project(ParamHelperExamples VERSION 0.0.1 LANGUAGES CXX)

# Initialize some default paths
include(GNUInstallDirs)

# Define the minimum C++ standard that is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include libraries
find_package(ParamHelper CONFIG REQUIRED)

add_executable(ParamHelperExamples param_helper_examples.cpp)
target_link_libraries(ParamHelperExamples PUBLIC paramhelper::paramhelper)

# Print a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)