cmake_minimum_required(VERSION 3.11)

set(SRC_PATH ${PROJECT_SOURCE_DIR}/codec)
include_directories(${SRC_PATH})

file(GLOB_RECURSE TMP_HEADER_LIST ${SRC_PATH}/**.h)
foreach(header ${TMP_HEADER_LIST})
  string(REPLACE "/" ";" arr ${header})
  list(GET arr -1 file_name)
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_if_different ${header} ${INC_PATH}/codec/${file_name})
endforeach()

file(GLOB_RECURSE SOURCE_LIST ${SRC_PATH}/**.c ${SRC_PATH}/**.cc ${SRC_PATH}/**.inc)

set(STATIC_LIB_NAME ${PROJECT_NAME}-codec-static)
set(SHARED_LIB_NAME ${PROJECT_NAME}-codec-shared)

add_library(
  ${STATIC_LIB_NAME} STATIC
  ${SOURCE_LIST}
)

add_library(
  ${SHARED_LIB_NAME} SHARED
  ${SOURCE_LIST}
)

target_link_libraries(${STATIC_LIB_NAME} PUBLIC saturn-static)
target_link_libraries(${SHARED_LIB_NAME} PUBLIC saturn-static)

unset(${STATIC_LIB_NAME})
unset(${SHARED_LIB_NAME})