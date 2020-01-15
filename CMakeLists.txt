#<===<===<===<===<===<===<===<===<===~===>===>===>===>===>===>===>===>===>
## File Name:    CMakeLists.txt
## Author:       Hao-Kai SUN
## Created:      2020-01-14 Tue 20:06:56 CST
## <<=====================================>>
## Last Updated: 2020-01-14 Tue 22:05:44 CST
##           By: Hao-Kai SUN
##     Update #: 21
## <<======== COPYRIGHT && LICENSE =======>>
##
## Copyright © 2020 SUN Hao-Kai <spin.hk@outlook.com>. All rights reserved.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or (at
## your option) any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with GNU Emacs.  If not, see <https://www.gnu.org/licenses/>.
##
## ============================== CODES ==============================>>>
cmake_minimum_required(VERSION 3.15)

# apply CMake policies for specific CMake versions
if(${CMAKE_VERSION} VERSION_LESS 3.15)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.16)
endif()

project(
    QuickFit
    VERSION 0.0.2
    DESCRIPTION "QuickFit library based on ROOT."
    LANGUAGES CXX
    )

set(CMAKE_CXX_STANDARD 17)

# output compile_commands.json for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set compiler for C
find_program(CMAKE_C_COMPILER gcc)
set(CMAKE_C_COMPILER
    "${CMAKE_C_COMPILER}"
    CACHE FILEPATH
    "The compiler for C language"
    FORCE
    )

set(CMAKE_C_FLAGS
    "-fPIC"
    CACHE STRING
    "The compiling flags for C compiler."
    FORCE
    )

# Set compiler for CPP
find_program(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_COMPILER
    "${CMAKE_CXX_COMPILER}"
    CACHE FILEPATH
    "The compiler for C++ language"
    FORCE
    )

set(CMAKE_CXX_FLAGS
    "-fPIC"
    CACHE STRING
    "The compiling flags for C++ compiler."
    )

# for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

## locate ROOT and defines a number of variables
find_package(ROOT REQUIRED COMPONENTS EG RooFit RooFitCore RooStats Minuit Foam Gui)
## define useful ROOT functions and macros
include(${ROOT_USE_FILE})
ROOT_GENERATE_DICTIONARY("G__${PACKAGE_NAME}" QuickFit.hh LINKDEF QuickFit_LinkDef.h)

# make shared library
add_library(
    ${PROJECT_NAME} SHARED
    src/PlotFormat.cc
    src/QuickFit.cc
    )

target_include_directories(
    ${PROJECT_NAME} BEFORE PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    )

target_compile_options(${PROJECT_NAME} PUBLIC
    ${ROOT_CXX_FLAGS}
    )

target_link_options(${PROJECT_NAME} PUBLIC
    -pthread -lm -ldl -rdynamic
    )

target_link_libraries(${PROJECT_NAME}
    PUBLIC ${ROOT_LIBRARIES})


#===================================================================<<<
#====================== CMakeLists.txt ends here ======================
