project(BlueBrick-Loader VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

add_library(${PROJECT_NAME} SHARED
	src/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME "BlueBrick"
)

find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${DETOURS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${DETOURS_LIBRARY})

add_compile_definitions(TARGET_GAME="${TARGET_GAME}")
target_include_directories(${PROJECT_NAME} PRIVATE "include/Games/${TARGET_GAME}/")

add_subdirectory("launcher")