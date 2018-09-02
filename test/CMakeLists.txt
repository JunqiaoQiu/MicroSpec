cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)

project(test)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -pthread") 
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories (../include)

aux_source_directory (./ PROJECT_ROOT)

aux_source_directory (../src SRC)

add_executable (${PROJECT_NAME} ${PROJECT_ROOT} ${SRC})
