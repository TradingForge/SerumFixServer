
set(CMAKE_VERBOSE_MAKEFILE ON)

cmake_minimum_required(VERSION 3.10)
project(fixserver
    LANGUAGES C CXX 
    VERSION 0.1.0)

include(ExternalProject)

ExternalProject_Add(
    P7
    PREFIX "${CMAKE_SOURCE_DIR}/libP7Client_v5.3"
    SOURCE_DIR "${CMAKE_SOURCE_DIR}/libP7Client_v5.3"
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ""
    BUILD_COMMAND /bin/sh build.sh
    INSTALL_COMMAND "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(FIX8_INCLUDE_DIRS /usr/local/include/fix8)
set(P7_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/libP7Client_v5.3/Sources)

string(CONCAT SERUM_NAME ${PROJECT_NAME} "serum")
string(CONCAT CLI_NAME ${PROJECT_NAME} "cli")
set(SHAREDLIB_NAME "shared")
set(MARKETLIB_NAME "market")

message(STATUS CMAKE_SOURCE_DIR:${CMAKE_SOURCE_DIR})
message(STATUS PROJECT_NAME:${PROJECT_NAME})
message(STATUS SERUM_NAME:${SERUM_NAME})
message(STATUS CLI_NAME:${CLI_NAME})
message(STATUS FIX8_INCLUDE_DIRS:${FIX8_INCLUDE_DIRS})
message(STATUS P7_INCLUDE_DIRS:${P7_INCLUDE_DIRS})
message(STATUS PROJECT_SOURCE_DIR:${PROJECT_SOURCE_DIR})
message(STATUS PROJECT_BINARY_DIR:${PROJECT_BINARY_DIR})

message(STATUS "\n")

# c++ 17
set (CMAKE_CXX_STANDARD 17)

# boost v. latest
#enable_testing()

add_subdirectory(serum)
add_subdirectory(marketlib)
add_subdirectory(sharedlib)
