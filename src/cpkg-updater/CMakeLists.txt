project(cpkg-updater VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES src/entry.cpp src/utils/version_info.cpp)

if(WIN32)
  set(PLATFORM)
  set(PLATFORM_LIB wininet)
else()
  set(PLATFORM)
  set(PLATFORM_LIB)
endif()

include_directories(${INCLUDES_DIR} includes)

add_executable(cpkg-updater ${SOURCES})

target_link_libraries(cpkg-updater PRIVATE CLI11 ${PLATFORM_LIB})
