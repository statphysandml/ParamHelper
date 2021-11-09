add_library(ParamHelper STATIC
        filesystem.cpp
        params.cpp)
target_include_directories(ParamHelper PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
