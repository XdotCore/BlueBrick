cmake_minimum_required(VERSION 3.12)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded)
cmake_policy(SET CMP0141 NEW)

set(CMAKE_CXX_STANDARD 20)

set(BLUEBRICK_VERSION 0.1.0)

project(BlueBrick VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

add_subdirectory("loader")