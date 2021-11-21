cmake_minimum_required(VERSION 3.15)

# Set a name and a version number for your project:
project(ParamHelperExamples VERSION 0.0.1 LANGUAGES CXX)

# Initialize some default paths
include(GNUInstallDirs)

# Define the minimum C++ standard that is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include libraries
find_library(ParamHelper NAMES libparamhelper.a PATHS ../build/src)
message("ParamHelper = ${ParamHelper}")
include_directories(../include/)

# This prints a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)

add_executable(ParamHelperExamples param_helper_examples.cpp)
target_link_libraries(ParamHelperExamples PUBLIC ${ParamHelper})