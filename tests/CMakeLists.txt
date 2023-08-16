cmake_minimum_required(VERSION 3.16)
project(co_requests_test)

include(${CMAKE_SOURCE_DIR}/cmake/compiler_options.cmake)

set(TEST_SRCS
  *.cpp
)

add_executable(unittest ${TEST_SRCS})
target_link_libraries(unittest CRQ::CRQ)
add_warnings_optimizations(unittest)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set_target_properties(unittest PROPERTIES COMPILE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")
  target_link_libraries(unittest gcov)
endif()

# add_subdirectory(template)
# add_subdirectory(multi_file)
# add_subdirectory(external_definition)
# if ("ssl" IN_LIST CROW_FEATURES)
# 	add_subdirectory(ssl)
# endif()
# add_subdirectory(img)
