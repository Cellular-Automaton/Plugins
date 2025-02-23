cmake_minimum_required(VERSION 3.12)

project(cami)

# standard version cpp
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

# change library prefix to none
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_STATIC_LIBRARY_PREFIX "")

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 1)

# executables will be at the root of the folder
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# options of the project
option(CAMI_BUILD_TESTS "Build the tests" OFF)
option(CAMI_BUILD_BENCHMARKS "Build benchmarks" OFF)

include(FetchContent)

# set colors for the messages because why not ?
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(ColourBold  "${Esc}[1m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
    set(Blue        "${Esc}[34m")
    set(Magenta     "${Esc}[35m")
    set(Cyan        "${Esc}[36m")
    set(White       "${Esc}[37m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
    set(BoldYellow  "${Esc}[1;33m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldMagenta "${Esc}[1;35m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldWhite   "${Esc}[1;37m")
endif()

# log library
message("${BoldWhite}Loading log library")
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG        "v1.x"
    SYSTEM
)
FetchContent_MakeAvailable(spdlog)
message("${BoldWhite}Spdlog loaded.")

# Warning flags
set(GCC_WARNING_FLAGS
    -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches
    -Wlogical-op -Wno-null-dereference -Wuseless-cast -Wno-pragmas)

set(CLANG_WARNING_FLAGS
    -Weverything
    -Wno-c++20-compat
    -Wno-c++98-c++11-compat-binary-literal
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-c99-extensions
    -Wno-disabled-macro-expansion
    -Wno-documentation-unknown-command
    -Wno-exit-time-destructors
    -Wno-missing-prototypes
    -Wno-global-constructors
    -Wno-implicit-int-float-conversion
    -Wno-return-std-move
    -Wno-documentation
    -Wno-deprecated-copy-with-user-provided-dtor
    -Wno-return-type-c-linkage
    -Wno-suggest-destructor-override
    -Wno-signed-enum-bitfield
    -Wno-weak-vtables)

if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS 13.0.0)
    set(CLANG_WARNING_FLAGS "${CLANG_WARNING_FLAGS} -Wno-return-std-move-in-c++11")
endif()

if (NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -std=c++20")

    set(WARNING_FLAGS
        -Wall
        -Wextra
        -Wshadow
        -Wnon-virtual-dtor
        -pedantic
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Wno-switch-default
        -Woverloaded-virtual
        -Wpedantic
        -Wconversion
        -Wsign-conversion
        -Wdouble-promotion
        -Wformat=2
        -Wno-effc++
        -Wno-suggest-override
        -Wno-null-dereference
        -Wno-zero-as-null-pointer-constant
        -Wno-padded
        -Wno-language-extension-token
    )
endif ()

if (CAMI_BUILD_TESTS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

# set different flags
set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}
  CACHE STRING
  "Choose the type of build, options are: None Debug Release tsan"
  FORCE)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")
elseif (CMAKE_BUILD_TYPE MATCHES "tsan")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=thread -O2 -g")
elseif (CMAKE_BUILD_TYPE MATCHES "Release" AND NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-exception-parameter -Wno-unused-variable")
endif ()

# Clang-tidy
if(USE_CLANG_TIDY)
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable")
  if(CLANG_TIDY_EXE)
      if(DEFINED CAMI_CLANG_TIDY_FLAGS)
          set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE} ${CAMI_CLANG_TIDY_FLAGS}")
    else()
      set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif()
    message("-- Found clang-tidy: ${CLANG_TIDY_EXE}")
  else()
    message("-- Failed to find clang-tidy")
  endif()
endif()

# set warning flags
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(WARNING_FLAGS ${WARNING_FLAGS} ${CLANG_WARNING_FLAGS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(WARNING_FLAGS ${WARNING_FLAGS} ${GCC_WARNING_FLAGS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(WARNING_FLAGS ${WARNING_FLAGS})
else()
  error("${CMAKE_CXX_COMPILER_ID}: Unsupported compiler.")
endif()

message("Warning flags: ${Cyan} ${WARNING_FLAGS}${ColourReset}")

# for library compilation => shared libraries of game engine, utils, etc
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/libs)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

## compile the project

# include directories
include_directories(include/)


# dynamic library with source files - easier to test
add_library(
    cami-core SHARED
    src/launch.cpp
    PLC/GameOfLife/GameOfLife.cpp
    PLC/Wolfram/Wolfram.cpp
    PLC/Lenia/Lenia.cpp
    PLC/MultiCouple/MultiCouple.cpp
)
target_compile_features(cami-core PRIVATE cxx_std_20)
target_compile_options(cami-core PRIVATE ${WARNING_FLAGS})
set_property(TARGET cami-core PROPERTY CXX_STANDARD_REQUIRED ON)
target_link_libraries(
    cami-core PRIVATE
    spdlog::spdlog
)

message("${BoldWhite}Finished loading cami-core${ColourReset}")

# add the executable with the main
add_executable(
    plugins_cami
    src/main.cpp
)
# link the libraries to executable
target_link_libraries(
    plugins_cami PRIVATE
    cami-core
    spdlog::spdlog
)
if (NOT WIN32)
    target_link_options(plugins_cami PRIVATE -rdynamic)
endif ()
target_compile_features(plugins_cami PRIVATE cxx_std_20)
target_compile_options(plugins_cami PRIVATE ${WARNING_FLAGS})
set_property(TARGET plugins_cami PROPERTY CXX_STANDARD 20)
set_property(TARGET plugins_cami PROPERTY CXX_STANDARD_REQUIRED ON)

# NOTE run test if option activated
if (CAMI_BUILD_TESTS)
    message("${BoldYellow}Building tests${ColourReset}")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "v1.14.x"
        OVERRIDE_FIND_PACKAGE)
    if (WIN32)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests/)
endif()

# NOTE run benchmarks if option activated
if (CAMI_BUILD_BENCHMARKS)
    message("${BoldYellow}Building benchmarks${ColourReset}")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG "v1.14.x"
        OVERRIDE_FIND_PACKAGE)
    if (WIN32)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(benchmarks)
endif()
