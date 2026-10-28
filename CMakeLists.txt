cmake_minimum_required(VERSION 3.20)
project(QuantStarter VERSION 1.0.0 LANGUAGES CXX)
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include(CTest)
option(QUANT_BUILD_BENCHMARKS "Build Google Benchmark executable" ON)

add_library(quant_analytics src/analytics.cpp)
target_include_directories(quant_analytics PUBLIC "${PROJECT_SOURCE_DIR}/include")
if(MSVC)
    target_compile_options(quant_analytics PRIVATE /W4 /permissive-)
else()
    target_compile_options(quant_analytics PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(FetchContent)
if(BUILD_TESTING)
    FetchContent_Declare(Catch2
        URL "${PROJECT_SOURCE_DIR}/third_party/Catch2-v3.7.1.tar.gz"
        URL_HASH SHA256=c991b247a1a0d7bb9c39aa35faf0fe9e19764213f28ffba3109388e62ee0269c
    )
    FetchContent_MakeAvailable(Catch2)
    add_executable(quant_tests tests/analytics_test.cpp)
    target_link_libraries(quant_tests PRIVATE quant_analytics Catch2::Catch2WithMain)
    list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")
    include(Catch)
    catch_discover_tests(quant_tests)
endif()

add_executable(quant_demo app/main.cpp)
target_link_libraries(quant_demo PRIVATE quant_analytics)

if(QUANT_BUILD_BENCHMARKS)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(benchmark
        URL "${PROJECT_SOURCE_DIR}/third_party/benchmark-v1.9.1.tar.gz"
        URL_HASH SHA256=32131c08ee31eeff2c8968d7e874f3cb648034377dfc32a4c377fa8796d84981
    )
    FetchContent_MakeAvailable(benchmark)
    add_executable(quant_benchmark benchmarks/analytics_benchmark.cpp)
    target_link_libraries(quant_benchmark PRIVATE quant_analytics benchmark::benchmark)
endif()

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    set(QUANT_DOC_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/docs")
    configure_file(Doxyfile Doxyfile @ONLY)
    add_custom_target(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Generating Doxygen HTML documentation"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found: install it and reconfigure to enable target docs")
endif()
