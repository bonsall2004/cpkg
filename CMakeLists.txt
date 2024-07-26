cmake_minimum_required(VERSION 3.28)
project(cpkg VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/binaries/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/intermediate/${CMAKE_BUILD_TYPE})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/intermediate/${CMAKE_BUILD_TYPE})
set(CMAKE_CACHEFILE_DIR ${CMAKE_SOURCE_DIR}/intermediate/${CMAKE_BUILD_TYPE})
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/intermediate/${CMAKE_BUILD_TYPE})

set(INCLUDES_DIR ${CMAKE_SOURCE_DIR}/includes)
set(DEPS_DIR     ${CMAKE_SOURCE_DIR}/deps)
set(SRC_DIR      ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(deps)
add_subdirectory(src)

add_custom_target(cpkg-all ALL DEPENDS CLI11 cpkg cpkg-core)