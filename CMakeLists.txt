cmake_minimum_required(VERSION 3.11)

project(advanced_vector CXX)
set(CMAKE_CXX_STANDARD 17) 

set(SOURCE_DIR advanced-vector)

if (CMAKE_SYSTEM_NAME MATCHES "^MINGW")
    set(SYSTEM_LIBS -lstdc++)
else()
    set(SYSTEM_LIBS)
endif()

file(GLOB sources
  ${SOURCE_DIR}/*.cpp
  ${SOURCE_DIR}/*.h
)

add_executable(advanced_vector ${sources}) 