cmake_minimum_required(VERSION 3.20)
project(dsa_atlas LANGUAGES CXX)

enable_testing()

add_executable(dsa_atlas_tests tests/test_algorithms.cpp)
target_include_directories(dsa_atlas_tests PRIVATE include)
target_compile_features(dsa_atlas_tests PRIVATE cxx_std_17)

if(MSVC)
  target_compile_options(dsa_atlas_tests PRIVATE /W4 /permissive-)
else()
  target_compile_options(dsa_atlas_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME dsa_atlas_tests COMMAND dsa_atlas_tests)
