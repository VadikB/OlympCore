cmake_minimum_required(VERSION 3.16)
project(olympcore VERSION 0.1.0 LANGUAGES CXX)

option(OLYMPCORE_BUILD_TESTS "Build olympcore tests" ON)

add_library(olympcore
    src/stats.cpp
)
target_include_directories(olympcore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_compile_features(olympcore PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(olympcore PRIVATE /W4)
else()
    target_compile_options(olympcore PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(olympcore_demo apps/olympcore_demo.cpp)
target_link_libraries(olympcore_demo PRIVATE olympcore)

if(OLYMPCORE_BUILD_TESTS)
    enable_testing()
    add_executable(test_stats tests/test_stats.cpp)
    target_link_libraries(test_stats PRIVATE olympcore)
    add_test(NAME test_stats COMMAND test_stats)
endif()
