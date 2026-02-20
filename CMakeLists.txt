cmake_minimum_required(VERSION 3.14...3.31)
project(dirpak LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(SOURCES
	dirpak.cc
)

add_executable(dirpak ${SOURCES})

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(dirpak PRIVATE -O3 -Wall -Wextra -Werror=return-type -fno-rtti -fno-exceptions)
endif()
