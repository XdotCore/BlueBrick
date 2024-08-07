project(TestMod VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

add_library(TestMod SHARED "Mod.cpp")

target_link_libraries(TestMod PRIVATE BlueBrick-Loader)
target_include_directories(TestMod PRIVATE
	"../../include/Games/${TARGET_GAME}/"
	"../../include/Universal/"
)
