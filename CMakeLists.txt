##
## @copyright Copyright (c) 2016-, Issam SAID <said.issam@gmail.com>
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted provided that the following conditions
## are met:
##
## 1. Redistributions of source code must retain the above copyright
##    notice, this list of conditions and the following disclaimer.
## 2. Redistributions in binary form must reproduce the above copyright
##    notice, this list of conditions and the following disclaimer in the
##    documentation and/or other materials provided with the distribution.
## 3. Neither the name of the copyright holder nor the names of its contributors
##    may be used to endorse or promote products derived from this software
##    without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
## INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY AND FITNESS
## FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE copyright 
## holder OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
## SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
## PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
## PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
## LIABILITY, WETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
## NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
## SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
##
## @file csv_compressor/CMakeLists.txt
## @author Issam SAID
## @brief CMake main build script for csv_compressor project.
##
project (csv_compressor-distribution C CXX)
cmake_minimum_required (VERSION 2.8)

## Enable ExternalProject CMake module.
include(ExternalProject)
find_package(Threads REQUIRED)

## Overwrite the install prefix.
set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}")

## Add extra cmake modules dir.
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

## Configurable options for how we want to build csv_compressor.
option(debug       
    "Build csv_compressor with the debug mode."            OFF)
option(verbose     
    "Build csv_compressor with the verbose mode activated." ON)

## Set the build type (DEFAULT is Release).
if (NOT CMAKE_BUILD_TYPE)
   set (CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)
if (debug)
	 set (CMAKE_BUILD_TYPE Debug)
endif (debug)
message(STATUS "... CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

## Set the compilation flags.
if ("${CMAKE_CXX_COMPILER_ID}"     MATCHES "Cray")
  	include(${CMAKE_MODULE_PATH}/cray_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
	include(${CMAKE_MODULE_PATH}/gnu_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    include(${CMAKE_MODULE_PATH}/intel_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "PGI")
    include(${CMAKE_MODULE_PATH}/pgi_cxx.cmake)
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
  	include(${CMAKE_MODULE_PATH}/msvc_cxx.cmake)
endif ("${CMAKE_CXX_COMPILER_ID}"     MATCHES "Cray")

## Add verbose compilation flags if required by the user.
if (verbose)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__VERBOSE")
endif (verbose)

## Use uparser: a library (author Issam SAID) that is  
## used to parse command line arguments and compiled
## separately for the sake of clarity, if the user sets 
## its path or download and install it as an external project.
if (DEFINED ENV{UPARSER_DIR})
    set(UPARSER_DIR "$ENV{UPARSER_DIR}")
elseif (IS_DIRECTORY ${CMAKE_SOURCE_DIR}/utilities/uparser)
    set(UPARSER_DIR "${CMAKE_SOURCE_DIR}/utilities/uparser")
    ExternalProject_Add(
        uparser
        PREFIX      ${UPARSER_DIR}
        BINARY_DIR  ${UPARSER_DIR}/build
        SOURCE_DIR  ${UPARSER_DIR}
        INSTALL_DIR ${UPARSER_DIR}
        DOWNLOAD_COMMAND ""
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>)
else ()
    ExternalProject_Add(
        uparser
        GIT_REPOSITORY https://github.com/issamsaid/uparser.git
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}
        UPDATE_COMMAND  ""
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>)
    ExternalProject_Get_Property(uparser SOURCE_DIR BINARY_DIR)
    set(UPARSER_DIR ${SOURCE_DIR})
endif ()

## Add uparser an expose it to csv_compressor.
add_library(libuparser IMPORTED STATIC GLOBAL)
set_target_properties(libuparser PROPERTIES
    "IMPORTED_LOCATION" "${UPARSER_DIR}/lib/libuparser.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}")
include_directories("${UPARSER_DIR}/include")

## Use the C++11 standard.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")

## Add MPI.
## find_package(MPI REQUIRED)
## set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
## set(CMAKE_CXX_LINK_FLAGS    ${CMAKE_CXX_LINK_FLAGS}    ${MPI_LINK_FLAGS})
## include_directories(${MPI_INCLUDE_PATH})

## Build the csv_compressor.
add_subdirectory(src)

## Cmake workaround for the lack of post_install steps:
## Add the path to uparser which we need during post install.
get_target_property(UPARSER_LIB libuparser IMPORTED_LOCATION)
install(CODE "set(uparser_LIB \"${uparser_LIB}\")")

## Add a subdirectory that contains the post-install scripts.
add_subdirectory(${CMAKE_SOURCE_DIR}/cmake/post_install)