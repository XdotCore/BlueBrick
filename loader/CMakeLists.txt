project(BlueBrick-Loader VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

add_library(${PROJECT_NAME} SHARED
	"src/main.cpp"
	"src/Logger/Logger.cpp"
	"src/Mod/Mod.cpp")
set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME "BlueBrick"
)

include(FetchContent)

FetchContent_Declare(
    rcmp
    GIT_REPOSITORY "https://github.com/Smertig/rcmp.git"
    GIT_TAG "origin/master"
)
FetchContent_MakeAvailable(rcmp)
target_link_libraries(${PROJECT_NAME} PRIVATE rcmp)

add_compile_definitions(TARGET_GAME="${TARGET_GAME}")
target_include_directories(${PROJECT_NAME} PRIVATE "include/Games/${TARGET_GAME}/")
target_include_directories(${PROJECT_NAME} PRIVATE "include/Universal/")

add_subdirectory("launcher")