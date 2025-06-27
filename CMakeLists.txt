# parallel_benchmark/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(parallel_benchmark VERSION 1.0.0 LANGUAGES CXX)

# Create the shared library
add_library(parallel_benchmark SHARED
    parallel_benchmark.cpp
)

# Specify include directories for the library itself
target_include_directories(parallel_benchmark PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Install rules for the library
install(TARGETS parallel_benchmark
    EXPORT parallel_benchmark_targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin # For Windows DLLs
)

# Install headers
install(FILES
    parallel_benchmark.h
    DESTINATION include
)

# Generate and install the package configuration file
# This is crucial for find_package to work correctly
install(EXPORT parallel_benchmark_targets
    FILE parallel_benchmarkConfig.cmake
    NAMESPACE parallel_benchmark::
    DESTINATION lib/cmake/parallel_benchmark
)

# Create a package configuration file for find_package(parallel_benchmark)
# This content will be written to parallel_benchmarkConfigVersion.cmake and lib_aConfig.cmake during install
# For simple cases, parallel_benchmarkConfig.cmake might just contain include(lib_aTargets.cmake)
# For more complex cases, you might generate it with configure_file
# Example simplified content for find_package(parallel_benchmark CONFIG)
set(parallel_benchmark_VERSION ${PROJECT_VERSION})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/parallel_benchmarkConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/parallel_benchmarkConfig.cmake @ONLY
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/parallel_benchmarkConfig.cmake
    DESTINATION lib/cmake/parallel_benchmark
)
