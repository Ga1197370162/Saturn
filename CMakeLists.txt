cmake_minimum_required(VERSION 3.11)

project(saturn VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "build type")
set(CMAKE_SKIP_RPATH TRUE)

if (MSVC)
  add_compile_options(/std:c++14)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /MP /wd4200")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /wd4200 /std:c++14")
elseif (UNIX)
  set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}  -g -Wall -fPIC -pipe -std=gnu90 -pthread -lrt")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -fPIC -pipe -std=c++14 -fno-exceptions -Wno-invalid-offsetof -pthread -lrt")
endif()

set(BIN_PATH ${PROJECT_SOURCE_DIR}/bin CACHE PATH "saturn bin")
set(INC_PATH ${BIN_PATH}/include CACHE PATH "saturn include")
set(LIB_PATH ${BIN_PATH}/lib CACHE PATH "saturn lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})

include_directories(${INC_PATH})

add_subdirectory(${PROJECT_SOURCE_DIR}/saturn)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests)
