cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(MicroSpec)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -mavx2 -msse4") 
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories (./include)

# It will find all files with specific extension names
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME} ${SRC})

add_subdirectory(test)

