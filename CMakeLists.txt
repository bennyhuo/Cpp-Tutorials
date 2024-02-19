cmake_minimum_required(VERSION 3.16.3)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 20)

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()
set(CMAKE_CXX_FLAGS "-O0 -fno-elide-constructors")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

message(${CMAKE_CXX_FLAGS})
message(${CMAKE_CXX_FLAGS_DEBUG})
# list all c files in current dir recursively.
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(file ${files})
    get_filename_component(name ${file} NAME)
    add_executable(${name} ${file})
#    target_link_libraries(${name} ${CONAN_LIBS})
endforeach()