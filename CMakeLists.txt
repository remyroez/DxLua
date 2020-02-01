cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0076 NEW)

project(DxLua C CXX)

add_executable(DxLua WIN32 MACOSX_BUNDLE)
target_compile_features(DxLua PRIVATE cxx_std_17)
target_include_directories(DxLua PRIVATE include)
target_link_directories(DxLua PRIVATE lib)

target_link_libraries(DxLua PRIVATE
  luajit lua51
)

if (MSVC)
  set_target_properties(DxLua PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

add_subdirectory(src)
