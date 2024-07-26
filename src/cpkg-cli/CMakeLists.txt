project(cpkg-cli VERSION 1.0.0 LANGUAGES CXX)

set(CPKG_VERSION 0.0.1)


file(GLOB_RECURSE SUBCOMMAND_SOURCES "src/subcommands/*.cpp" "src/subcommands/*.hpp")

if(WIN32)
  set(PLATFORM src/program_info/windows/ProgramInfo.cpp
      src/updater/windows/updater.cpp)
  set(PLATFORM_LIB wininet)
else()
  set(PLATFORM src/program_info/linux/ProgramInfo.cpp
      src/updater/linux/updater.cpp)
  set(PLATFORM_LIB)
endif()

set(SOURCES
    src/main.cpp
    includes/CLIHelper.cpp
    includes/CLIHelper.hpp
    src/program_info/ProgramInfo.h
    src/utils/version.cpp
    src/updater/updater.h
    )

message(STATUS "cpkg-cli found! [ver ${CPKG_VERSION}]")
include_directories(${INCLUDES_DIR} includes src)

add_compile_definitions(CPKG_VERSION="${CPKG_VERSION}")

add_executable(cpkg ${SUBCOMMAND_SOURCES} ${SOURCES} ${PLATFORM})
target_link_libraries(cpkg PRIVATE cpkg-core CLI11 ${PLATFORM_LIB})
target_include_directories(cpkg PRIVATE ${DEPS_DIR}/CLI11/include)