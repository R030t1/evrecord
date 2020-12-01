cmake_minimum_required(
  VERSION 3.10.2)
project(
  evrecord
  VERSION 0.0.1)

set(CMAKE_MODULE_PATH "cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++2a")

find_package(Boost REQUIRED COMPONENTS
  iostreams)
include_directories(${Boost_INCLUDE_DIRS})

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
#protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS name.proto)

# Compile the constant to name tables separately as C.
add_library(evnames OBJECT names.c)

add_executable(evrecord evrecord.cc)
target_include_directories(
  evrecord PUBLIC
  "${PROJECT_BINARY_DIR}")
target_link_libraries(
  evrecord
  evnames
  Boost::iostreams
  ${Protobuf_LIBRARIES})
