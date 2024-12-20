##
## CMakeLists: benchmarks
##

cmake_minimum_required(VERSION 3.12)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/benchmarks)

message("In the cmake to build benchmarks")

include(FetchContent)

option(BENCHMARK_DOWNLOAD_DEPENDENCIES "download its dependencies" ON)
# option(GOOGLETEST_PATH "path to googletests" ${CMAKE_BINARY_DIR}/_deps/)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY "https://github.com/google/benchmark.git"
    GIT_TAG "main"
    SYSTEM
)

FetchContent_MakeAvailable(googlebenchmark)

add_executable(
    benchmarks_plugins_cami
    src/benchmarks.cpp
    src/BM_GameOfLife.cpp
)

target_compile_options(benchmarks_plugins_cami PRIVATE ${WARNING_FLAGS})
target_include_directories(benchmarks_plugins_cami PUBLIC ${PROJECT_SOURCE_DIR}/PLC/include)
target_link_libraries(
    benchmarks_plugins_cami PRIVATE
    cami-core
    GTest::gtest_main
    benchmark::benchmark
)

set_target_properties(benchmarks_plugins_cami PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
message("Should have built benchmarks")
