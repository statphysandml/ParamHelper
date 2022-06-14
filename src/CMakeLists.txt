add_library(paramhelper STATIC
        filesystem.cpp
        params.cpp
        project.cpp)

target_include_directories(paramhelper PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${json_SOURCE_DIR}/include/>
)