cmake_minimum_required(VERSION 3.18)
project(paramhelper VERSION 1.0.0 LANGUAGES CXX)

# Options
option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)
option(PARAMHELPER_USE_SYSTEM_JSON "Use system-installed nlohmann_json" OFF)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Handle nlohmann_json dependency
if(PARAMHELPER_USE_SYSTEM_JSON)
    # Check if JSON_INCLUDE_DIR is provided by the superbuild
    if(DEFINED JSON_INCLUDE_DIR)
        message(STATUS "Using JSON include directory provided by superbuild: ${JSON_INCLUDE_DIR}")
        # Simple interface library to handle the include directory
        add_library(json_interface INTERFACE)
        target_include_directories(json_interface INTERFACE ${JSON_INCLUDE_DIR})
        set(JSON_TARGET json_interface)
    else()
        # Find system package
        message(STATUS "Finding system nlohmann_json package")
        find_package(nlohmann_json REQUIRED)
        set(JSON_TARGET nlohmann_json::nlohmann_json)
    endif()
else()
    # Fetch our own copy
    message(STATUS "Fetching nlohmann_json")
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(nlohmann_json)
    set(JSON_TARGET nlohmann_json::nlohmann_json)
    
    # Note: When not using system JSON, we'll need to ensure the headers are installed
    set(INSTALL_JSON_HEADERS TRUE)
endif()

# Define the library
add_library(
    paramhelper
    src/filesystem.cpp
    src/params.cpp
    src/project.cpp
)

# Create an alias target for use within the build
add_library(${PROJECT_NAME}::paramhelper ALIAS paramhelper)

# Include directories
target_include_directories(paramhelper
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(paramhelper
    PUBLIC
        ${JSON_TARGET}
)

# Installation
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

# Install headers
install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# If we fetched JSON and need to install the headers
if(DEFINED INSTALL_JSON_HEADERS)
    install(
        DIRECTORY ${nlohmann_json_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endif()

# Install library
install(
    TARGETS paramhelper
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# If using a custom JSON interface target, install it too
if(PARAMHELPER_USE_SYSTEM_JSON AND DEFINED JSON_INCLUDE_DIR)
    install(
        TARGETS json_interface
        EXPORT ${PROJECT_NAME}Targets
    )
endif()

# Export targets
install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${INSTALL_CONFIGDIR}
)

# Create package config files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/paramhelperConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install config files
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

# Build tests if enabled
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# Build examples if enabled
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()