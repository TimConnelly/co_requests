cmake_minimum_required(VERSION 3.16)
project(co_requests_examples)

include(${CMAKE_SOURCE_DIR}/cmake/compiler_options.cmake)

add_executable(get get.cpp)
add_warnings_optimizations(get)
target_link_libraries(get PUBLIC 
    CRQ::CRQ
    httplib
    OpenSSL::SSL)
