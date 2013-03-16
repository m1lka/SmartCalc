cmake_minimum_required(VERSION 2.8)
project(Calculator_temporarily)
set(SOURCE_CODE main.cpp)
set(SOURCE_DIR ./src)
set(INCLUDE_DIR ./include)
include_directories(${INCLUDE_DIR})

set(SOURCE ${SOURCE_DIR}/parser.cpp
					 ${SOURCE_DIR}/log.cpp
					 ${SOURCE_DIR}/math/math_fucn.cpp)

set(HEADER ${INCLUDE_DIR}/parser.h
					 ${INCLUDE_DIR}/log.h
					 ${INCLUDE_DIR}/math/math_func.h)

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
	add_definitions(-D_lin)
	add_executable(calc_0.1.1 ${HEADER} ${SOURCE} ${SOURCE_CODE})
elseif($(CMAKE_SYSTEM_NAME) STREQUAL Windows)
	add_definitions(-D_win32)
	add_executable(calc_0.1.1 ${HEADER} ${SOURCE} ${SOURCE_CODE})
endif()
