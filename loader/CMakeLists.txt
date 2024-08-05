project(BlueBrick-Loader VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

file(GLOB SOURCES CONFIGURE_DEPENDS
	"src/*.cpp"
	"src/Logger/*.cpp"
	"src/Mod/*.cpp")

configure_file("src/version.rc.in" "version.rc")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/version.rc")

add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME "BlueBrick"
	VERSION ${BLUEBRICK_VERSION}
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
target_include_directories(${PROJECT_NAME} PRIVATE
	"include/Games/${TARGET_GAME}/"
	"include/Universal/"
)

add_compile_definitions(BLUEBRICK_EXPORTING)

add_subdirectory("launcher")
add_subdirectory("test")

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
install(TARGETS ProxyLauncher RUNTIME DESTINATION ".")
install(TARGETS ${PROJECT_NAME} DESTINATION "BlueBrick/")
install(DIRECTORY "include/Games/${TARGET_GAME}" DESTINATION "BlueBrick/include/")
install(DIRECTORY "include/Universal" DESTINATION "BlueBrick/include/")
if (COMPILE_TEST_MODS)
	install(TARGETS TestMod RUNTIME DESTINATION "BlueBrick/Mods/")
endif()
