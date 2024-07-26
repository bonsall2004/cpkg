project(cpkg-cli VERSION 1.0.0 LANGUAGES CXX)

set(CPKG_VERSION 1.0.0)


file(GLOB_RECURSE SUBCOMMAND_SOURCES "src/subcommands/*.cpp" "src/subcommands/*.hpp")

if(WIN32)
  set(PLATFORM src/program_info/windows/ProgramInfo.cpp)
else()
  set(PLATFORM src/program_info/linux/ProgramInfo.cpp)
endif()

set(SOURCES
    src/main.cpp
    includes/CLIHelper.cpp
    includes/CLIHelper.hpp
    src/program_info/ProgramInfo.h
    src/utils/version.cpp
    )

message(STATUS "cpkg-cli found! [ver ${CPKG_VERSION}]")
include_directories(${INCLUDES_DIR} includes)

add_compile_definitions(CPKG_VERSION="${CPKG_VERSION}")

add_executable(cpkg ${SUBCOMMAND_SOURCES} ${SOURCES} ${PLATFORM})
target_link_libraries(cpkg PRIVATE cpkg-core CLI11)
target_include_directories(cpkg PRIVATE ${DEPS_DIR}/CLI11/include)