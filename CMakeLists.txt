cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(MicroSpec)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "2")
SET(VERSION_PATCH "1")

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread") 
endif(CMAKE_COMPILER_IS_GNUCXX)

# Whether Allo some default or user-define actions during each step of DFA computation
option(ALLOW_ACTION "Allow Action during each DFA computation" OFF)
if(ALLOW_ACTION)
	add_definitions(-DALLOW_ACTION)
	message(STATUS "Allow Actions Right Now")
else()
	message(STATUS "Not Allow Actions Right Now")
endif()

# Whether AVX2 SIMD instructions are allowed
option(AVX2_SUPPORT "Support AVX2 instruction sets" ON)
if(AVX2_SUPPORT)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -msse4") 
	add_definitions(-DAVX2_SUPPORT)
	message(STATUS "AVX2 Support")
else()
	message(STATUS "AVX2 Not Support")
endif()


include_directories (./include)

# It will find all files with specific extension names
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${SRC})

add_subdirectory(test)

