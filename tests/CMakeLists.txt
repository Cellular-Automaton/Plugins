##
## CMakeLists: tests
##

cmake_minimum_required(VERSION 3.12)

enable_testing()

find_package(GTest REQUIRED)

add_executable(
    tests_plugins_cami
    test.cpp
    PLC/TestGameOfLife.cpp
    PLC/TestWolfram.cpp
    PLC/TestLenia.cpp
    )
target_compile_options(tests_plugins_cami PRIVATE ${WARNING_FLAGS})
target_include_directories(tests_plugins_cami PUBLIC ${PROJECT_SOURCE_DIR}/PLC/include)
target_link_libraries(
    tests_plugins_cami PRIVATE
    cami-core
    GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(tests_plugins_cami)
