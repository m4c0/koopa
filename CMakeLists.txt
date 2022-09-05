cmake_minimum_required(VERSION 3.23)
project(koopa VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(koopa INTERFACE)
target_compile_features(koopa INTERFACE cxx_std_20)
target_include_directories(koopa INTERFACE inc)

add_executable(test-koopa
  src/binary.cpp
  src/combiner.cpp
  src/constants.cpp
  src/dummy.cpp
  src/io.cpp 
  src/matcher.cpp
  src/str.cpp)
target_link_libraries(test-koopa PUBLIC koopa)
