if (NOT COMPILE_TEST_MODS)
	return()
endif()

if (GAME_OS STREQUAL "OS_WINDOWS")
	set(BLUEBRICK_DLL "__declspec(dllimport)")
else()
	set(BLUEBRICK_DLL)
endif()

add_compile_definitions(BLUEBRICK_DLL=${BLUEBRICK_DLL})
add_compile_definitions(IMGUI_API=${BLUEBRICK_DLL})

add_subdirectory("TestMod")
