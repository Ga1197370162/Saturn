cmake_minimum_required(VERSION 3.11)

project(gtest LANGUAGES C CXX)

include_directories(${TESTS_PATH}/gtest)
file(GLOB_RECURSE SOURCE_LIST ${TESTS_PATH}/gtest/**.cc)

set(STATIC_LIB_NAME gtest)

add_library(
  ${STATIC_LIB_NAME} STATIC
  ${SOURCE_LIST}
)

unset(STATIC_LIB_NAME)

