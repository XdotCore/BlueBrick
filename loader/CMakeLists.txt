project(BlueBrick-Loader VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

string(REPLACE "PLATFORM_" "" PLATFORM_NAME ${GAME_PLATFORM})
string(REPLACE "RENDERER_" "" RENDERER_NAME ${GAME_RENDERER})

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"src/*.cpp"
	"src/*.rc.in"
	"src/Universal/*.cpp"
	"src/Games/${TARGET_GAME}/*.cpp"
	"src/Internal/*.cpp"
	"src/Platform/${PLATFORM_NAME}/${RENDERER_NAME}/*.cpp"
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
	"include/Universal/*.hpp"
	"include/Games/${TARGET_GAME}/*.hpp"
	"include/Internal/*.hpp"
	"include/Platform/${PLATFORM_NAME}/${RENDERER_NAME}/*.hpp"
)

configure_file("src/version.rc.in" "version.rc")
set(SOURCES ${SOURCES} "${CMAKE_CURRENT_BINARY_DIR}/version.rc")

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME "BlueBrick"
	VERSION ${BLUEBRICK_VERSION}
)

# rcmp hooking
include(FetchContent)
FetchContent_Declare(
    rcmp
    GIT_REPOSITORY "https://github.com/Smertig/rcmp.git"
    GIT_TAG "v0.2.2"
)
FetchContent_MakeAvailable(rcmp)
target_link_libraries(${PROJECT_NAME} PUBLIC rcmp)

# imgui
file(GLOB IMGUI_SOURCES "imgui/*.cpp"
	"imgui/backends/imgui_impl_${PLATFORM_NAME}.cpp"
	"imgui/backends/imgui_impl_${RENDERER_NAME}.cpp"
)
add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "imgui/")
target_link_libraries(${PROJECT_NAME} PUBLIC imgui)

# compile definitions
add_compile_definitions(TARGET_GAME="${TARGET_GAME}")
add_compile_definitions(GAME_NAME="${GAME_NAME}")
add_compile_definitions(BLUEBRICK_VERSION="${BLUEBRICK_VERSION}")
add_compile_definitions(ARCH_x86=0)
add_compile_definitions(ARCH_x64=1)
add_compile_definitions(GAME_ARCH=${GAME_ARCH})
add_compile_definitions(PLATFORM_WIN32=0)
add_compile_definitions(GAME_PLATFORM=${GAME_PLATFORM})
add_compile_definitions(GAME_PLATFORM_NAME=${PLATFORM_NAME})
add_compile_definitions(RENDERER_DX9=0)
add_compile_definitions(GAME_RENDERER=${GAME_RENDERER})
add_compile_definitions(GAME_RENDERER_NAME=${RENDERER_NAME})
target_compile_definitions(${PROJECT_NAME} PRIVATE BLUEBRICK_EXPORTING)

# includes
target_include_directories(${PROJECT_NAME} PUBLIC
	"include/Games/${TARGET_GAME}/"
	"include/Universal/"
	PRIVATE
	"include/Internal/"
	"include/Platform/${PLATFORM_NAME}/${RENDERER_NAME}"
)

# subprojects
add_subdirectory("launcher")
add_subdirectory("test")

# install
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
install(TARGETS ProxyLauncher RUNTIME DESTINATION ".")
install(TARGETS ${PROJECT_NAME} DESTINATION "BlueBrick/")
if (COMPILE_TEST_MODS)
	install(TARGETS TestMod RUNTIME DESTINATION "BlueBrick/Mods/")
endif()
