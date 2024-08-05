#pragma once

#ifdef BLUEBRICK_EXPORTING
	#define BLUEBRICK_DLL __declspec(dllexport)
#else
	#define BLUEBRICK_DLL __declspec(dllimport)
#endif

#define BLUEBRICK_API extern "C" __declspec(dllexport)
