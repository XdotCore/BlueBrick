#pragma once

/**
* Auto-generates the entry function for loading the mod
* 
* e.g.
* $ModEntry(TestMod) {
*	...
* }
* 
* expands to
* 
* class TestMod;
* BLUEBRICK_API void* modEntry() {
*	return BlueBrick::Mod::Get<TestMod>();
* }
* class TestMod final : public BlueBrick::Mod {
*	...
* }
*/

#define $ModEntry(MOD_NAME) class MOD_NAME;\
	BLUEBRICK_API void* modEntry() { return BlueBrick::Mod::Get<MOD_NAME>(); } \
	class MOD_NAME final : public BlueBrick::Mod
