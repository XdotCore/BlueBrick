cmake_minimum_required(VERSION 3.28)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded)
cmake_policy(SET CMP0141 NEW)

set(CMAKE_CXX_STANDARD 20)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(BLUEBRICK_VERSION 0.1.0)

project(BlueBrick VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

add_subdirectory("loader")