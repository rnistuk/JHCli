cmake_minimum_required(VERSION 3.16)
project(Covid19Data)

set(CMAKE_CXX_STANDARD 20)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)
add_executable(Covid19Data main.cpp src/johns_hopkins.cpp src/johns_hopkins.h src/data_parser.cpp src/data_parser.h src/region.h)
#add_executable (curl-demo convert.cpp)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(Covid19Data ${CURL_LIBRARIES})

include(ExternalProject)
ExternalProject_Add(googletest
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}/googletest"
        URL https://github.com/google/googletest/archive/release-1.8.1.tar.gz
        URL_HASH SHA256=9bf1fe5182a604b4135edc1a425ae356c9ad15e9b23f9f12a02e80184c3a249c
        TIMEOUT 30
        DOWNLOAD_NO_PROGRESS true
        INSTALL_COMMAND "")


ExternalProject_Get_Property(googletest SOURCE_DIR)
include_directories(${SOURCE_DIR}/googlemock/include ${SOURCE_DIR}/googletest/include)
ExternalProject_Get_Property(googletest BINARY_DIR)
link_directories(${BINARY_DIR}/googlemock ${BINARY_DIR}/googlemock/gtest)
set(GMOCK_BOTH_LIBRARIES gmock_main gmock gtest)
set_property(DIRECTORY PROPERTY CLEAN_NO_CUSTOM "${CMAKE_CURRENT_BINARY_DIR}/googletest")
unset(SOURCE_DIR)
unset(BINARY_DIR)

enable_testing()
add_subdirectory(tests)
#add_subdirectory(tests/runners/)