#####################################
# Define Project-Wide Settings
#####################################
cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

include(FetchContent)

# Define the project name and language
project(co_requests
	LANGUAGES CXX
)

# Make sure Findasio.cmake module is found
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Check if CRQ is the main project
set(CRQ_IS_MAIN_PROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(CRQ_IS_MAIN_PROJECT ON)
endif()

# Set required C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Default to build type "Release" unless tests are being built
if(NOT CMAKE_BUILD_TYPE)
	if (NOT CRQ_BUILD_TESTS)
		message(STATUS "No build type selected, default to Release")
		set(CMAKE_BUILD_TYPE "Release")
	else()
		message(STATUS "No build type selected but tests are being built, default to Debug")
		set(CMAKE_BUILD_TYPE "Debug")
	endif()
endif()

#####################################
# Define Options
#####################################
option(CRQ_BUILD_EXAMPLES     "Build the examples in the project"      ${CRQ_IS_MAIN_PROJECT})
option(CRQ_BUILD_TESTS        "Build the tests in the project"          OFF) # ${CRQ_IS_MAIN_PROJECT})
# option(CRQ_AMALGAMATE         "Combine all headers into one"           OFF) # FUTURE
# option(CRQ_INSTALL            "Add install step for CRQ"               ON ) # FUTURE

find_package(OpenSSL REQUIRED)

FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        6e79e682b726f524310d55dec8ddac4e9c52fb5f # release-3.4.0
)

FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG        c7ed1796a778592ae5a122287a16b1dd4770858a # v0.13.3
)

FetchContent_MakeAvailable(catch2 httplib)

#####################################
# Define Targets
#####################################
add_library(CRQ INTERFACE)
add_library(CRQ::CRQ ALIAS CRQ)


target_include_directories(CRQ
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

# Tests
if(NOT MSVC AND CRQ_BUILD_TESTS)
	# if(NOT "compression" IN_LIST CRQ_FEATURES)
	# 	message(STATUS "Compression tests are omitted. (Configure with CRQ_FEATURES containing 'compression' to enable them)")
	# endif()
	# if(NOT "ssl" IN_LIST CRQ_FEATURES)
	# 	message(STATUS "SSL tests are omitted. (Configure with CRQ_FEATURES containing 'ssl' to enable them)")
	# else()
	# 	add_test(NAME ssl_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/ssl/ssltest)
	# endif()

	add_subdirectory(tests)
	enable_testing()
	add_test(NAME co_requests_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/unittest)
	# add_test(NAME template_test COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests/template/test.py WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests/template)

endif()

# Examples
if(CRQ_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()

