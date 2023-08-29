cmake_minimum_required(VERSION 3.11)

project(gtest LANGUAGES C CXX)

file(GLOB_RECURSE SOURCE_LIST ${THIRD_PATH}/gtest/**.cc)

set(STATIC_LUA_LIB_NAME gtest)

add_library(
  ${STATIC_LUA_LIB_NAME} STATIC
  ${SOURCE_LIST}
)

unset(STATIC_LUA_LIB_NAME)

