cmake_minimum_required(VERSION 3.20)
project(dsa_atlas LANGUAGES CXX)

enable_testing()

function(add_dsa_atlas_test_target target standard)
  add_executable(${target} tests/test_algorithms.cpp)
  target_include_directories(${target} PRIVATE include)
  set_target_properties(
    ${target}
    PROPERTIES
      CXX_STANDARD ${standard}
      CXX_STANDARD_REQUIRED YES
      CXX_EXTENSIONS NO
  )

  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
  endif()

  add_test(NAME ${target} COMMAND ${target})
endfunction()

add_dsa_atlas_test_target(dsa_atlas_tests_cpp11 11)
add_dsa_atlas_test_target(dsa_atlas_tests_cpp17 17)
