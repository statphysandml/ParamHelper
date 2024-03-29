cmake_minimum_required(VERSION 3.15)

project(ParamHelper VERSION 0.0.1 LANGUAGES CXX)

# Initialize some default paths
include(GNUInstallDirs)

# Define the minimum C++ standard that is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilation options
set(BUILD_DOCS ON CACHE BOOL "Enable building of documentation")

# Add nlohmann json
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
  GIT_TAG v3.0.0)
FetchContent_MakeAvailable(json)

# compile the library
add_subdirectory(src)

# compile the tests
include(CTest)
if(BUILD_TESTING)
  if(NOT TARGET Catch2)
    add_subdirectory(ext/Catch2)
    include(./ext/Catch2/contrib/Catch.cmake)
  endif()
  add_subdirectory(tests)
endif()

if(BUILD_DOCS)
  # Add the documentation
  add_subdirectory(doc)
endif()

# Add an alias target for use if this project is included as a subproject in another project
add_library(paramhelper::paramhelper ALIAS paramhelper)

# Install targets and configuration
install(
  TARGETS paramhelper
  EXPORT paramhelper-config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
  EXPORT paramhelper-config
  NAMESPACE paramhelper::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/paramhelper
)

install(
  DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${json_SOURCE_DIR}/include/nlohmann/json.hpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nlohmann/)

# This prints a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)
