cmake_minimum_required(VERSION 3.18)
project(paramhelper VERSION 1.0.0 LANGUAGES CXX)

# Options
option(BUILD_TESTING "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)
option(PARAMHELPER_USE_SYSTEM_JSON "Use system-installed nlohmann_json" OFF)
# Add this to recognize the variable passed from superbuild
if(DEFINED USE_SYSTEM_JSON)
    set(PARAMHELPER_USE_SYSTEM_JSON ${USE_SYSTEM_JSON})
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "PARAMHELPER_USE_SYSTEM_JSON=${PARAMHELPER_USE_SYSTEM_JSON}")
if(DEFINED JSON_INCLUDE_DIR)
    message(STATUS "JSON_INCLUDE_DIR is set to: ${JSON_INCLUDE_DIR}")
endif()

# Handle nlohmann_json dependency
if(PARAMHELPER_USE_SYSTEM_JSON)
    message(STATUS "Using system JSON")
    
    if(DEFINED JSON_INCLUDE_DIR)
        # Create a wrapper target that will be included in the export set
        add_library(paramhelper_json_wrapper INTERFACE)
        target_include_directories(paramhelper_json_wrapper INTERFACE 
            $<BUILD_INTERFACE:${JSON_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        )
        set(JSON_TARGET paramhelper_json_wrapper)
        set(EXPORT_JSON_TARGET TRUE)
    else()
        # When using standard system package, handle dependency in config file
        find_package(nlohmann_json REQUIRED)
        set(JSON_TARGET nlohmann_json::nlohmann_json)
        # We'll handle this dependency in the config file
        set(EXPORT_JSON_TARGET FALSE)
    endif()
else()
    # Download nlohmann_json at configure time
    message(STATUS "Fetching nlohmann_json")
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(nlohmann_json)
    
    # Create our own interface that will be included in the export set
    add_library(paramhelper_json_wrapper INTERFACE)
    target_include_directories(paramhelper_json_wrapper INTERFACE 
        $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    )
    
    # Install the headers
    install(
        DIRECTORY ${nlohmann_json_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
    
    set(JSON_TARGET paramhelper_json_wrapper)
    set(EXPORT_JSON_TARGET TRUE)
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

# Install targets - always install paramhelper
if(EXPORT_JSON_TARGET)
    # If we created our own JSON wrapper, install both together
    install(
        TARGETS paramhelper paramhelper_json_wrapper
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
else()
    # Otherwise just install paramhelper
    install(
        TARGETS paramhelper
        EXPORT ${PROJECT_NAME}Targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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