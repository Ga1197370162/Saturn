cmake_minimum_required(VERSION 3.11)

project(tests LANGUAGES C CXX)

set(TESTS_PATH ${PROJECT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

add_subdirectory(${TESTS_PATH}/gtest)
file(GLOB SOURCE_LIST ${TESTS_PATH}/**.cc)

set(TEST_PROJECT sat_global_test)
set(SATURN_LIB saturn-static gtest)

include_directories(${INC_PATH}/saturn)
include_directories(${TESTS_PATH}/gtest)

add_executable(${TEST_PROJECT} ${SOURCE_LIST})
target_link_libraries(${TEST_PROJECT} PUBLIC ${SATURN_LIB})
