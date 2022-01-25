cmake_minimum_required(VERSION 3.15)

project(ParamHelper VERSION 0.0.1 LANGUAGES CXX)

# Initialize some default paths
include(GNUInstallDirs)

# Define the minimum C++ standard that is required
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Compilation options
set(BUILD_DOCS ON CACHE BOOL "Enable building of documentation")

# compile the library
add_subdirectory(src)


# compile the tests
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(ext/Catch2)
  include(./ext/Catch2/contrib/Catch.cmake)
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

# This prints a summary of found dependencies
include(FeatureSummary)
feature_summary(WHAT ALL)
