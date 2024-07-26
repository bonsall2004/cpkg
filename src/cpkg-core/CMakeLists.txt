project(cpkg-core VERSION 0.0.1 LANGUAGES CXX)

set(CPKG_CORE_LIB_VERSION 0.0.1)

message(STATUS "cpkg-core found! [ver ${CPKG_CORE_LIB_VERSION}]")

set(SOURCES
    src/utils/utils.cpp)
include_directories(${INCLUDES_DIR})

add_library(cpkg-core SHARED ${SOURCES})

target_include_directories(cpkg-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)
target_compile_definitions(cpkg-core PUBLIC CPKG_CORE_LIB_VERSION="${CPKG_CORE_LIB_VERSION}")