project(TestMod VERSION ${BLUEBRICK_VERSION} LANGUAGES C CXX)

add_library(TestMod SHARED "Mod.cpp")

target_link_libraries(TestMod PRIVATE loader)
target_include_directories(TestMod PRIVATE
	"../../include/Games/${TARGET_GAME}/"
	"../../include/Universal/"
	"../../imgui/"
)

	install(TARGETS TestMod RUNTIME DESTINATION "BlueBrick/Mods/")
