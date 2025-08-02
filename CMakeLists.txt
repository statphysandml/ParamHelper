cmake_minimum_required(VERSION 3.18)

project(ParamHelper VERSION 0.0.1 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Install directories
include(GNUInstallDirs)

# Fetch nlohmann_json (modern FetchContent usage)
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG v3.0.0
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(json)

# Add the src directory, which defines the paramhelper target
add_subdirectory(src)

# Alias for downstream usage
add_library(paramhelper::paramhelper ALIAS paramhelper)

# Testing
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

# Documentation (optional)
option(BUILD_DOCS "Enable building of documentation" ON)
if(BUILD_DOCS)
  add_subdirectory(doc)
endif()

# === Installation ===

install(
  TARGETS paramhelper
  EXPORT paramhelper-config
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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

install(
  FILES ${json_SOURCE_DIR}/include/nlohmann/json.hpp
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nlohmann/
)

# Feature summary
include(FeatureSummary)
feature_summary(WHAT ALL)
