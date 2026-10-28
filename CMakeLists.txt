cmake_minimum_required(VERSION 3.20)
project(QuantStarter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(QUANT_BUILD_TESTS "Build unit tests" ON)
option(QUANT_BUILD_BENCHMARKS "Build Google Benchmark executable" ON)
option(QUANT_BUILD_DOCS "Enable the Doxygen docs target" OFF)

add_library(quant src/analytics.cpp)
target_include_directories(quant PUBLIC "${PROJECT_SOURCE_DIR}/include")
if(MSVC)
    target_compile_options(quant PRIVATE /W4 /WX)
else()
    target_compile_options(quant PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_executable(quant_demo src/main.cpp)
target_link_libraries(quant_demo PRIVATE quant)

if(QUANT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(QUANT_BUILD_BENCHMARKS)
    include(FetchContent)
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
    if(POLICY CMP0135)
        cmake_policy(SET CMP0135 NEW)
    endif()
    FetchContent_Declare(googlebenchmark
        URL "${PROJECT_SOURCE_DIR}/third_party/archives/benchmark-1.9.1.tar.gz"
        URL_HASH SHA256=32131c08ee31eeff2c8968d7e874f3cb648034377dfc32a4c377fa8796d84981
    )
    FetchContent_MakeAvailable(googlebenchmark)
    add_subdirectory(benchmarks)
endif()

if(QUANT_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    add_custom_target(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${PROJECT_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        COMMENT "Generating API documentation in docs/html"
        VERBATIM
    )
endif()
