
cmake_minimum_required(VERSION 2.8)


set(PROJECT smartcalc)

project(${PROJECT} CXX)

set(SRCDIR ${PROJECT_SOURCE_DIR}/src/)

aux_source_directory(${SRCDIR} SRC_LIST)

add_executable(${PROJECT} ${SRC_LIST})
