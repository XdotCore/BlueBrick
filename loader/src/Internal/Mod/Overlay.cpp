#include "Mod/Overlay.hpp"
#include "Overlay.hpp" // The platform specific one
#include "imgui.h"

#undef WIN32 // messes up preproc

#define CONCAT(a, b, c) a##_##b##_##c
#define PLATFORM_SPECIFIC_OVERLAY_IMPL(platform, renderer) CONCAT(Overlay, platform, renderer)
#define PLATFORM_SPECIFIC_OVERLAY PLATFORM_SPECIFIC_OVERLAY_IMPL(GAME_PLATFORM_NAME, GAME_RENDERER_NAME)

namespace BlueBrick {
	Overlay& Overlay::instance() {
		static PLATFORM_SPECIFIC_OVERLAY instance;
		return instance;
	}

	void Overlay::Start() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
	}

	void Overlay::Draw() {
		ImGui::NewFrame();

		if (showDemoWindow)
			ImGui::ShowDemoWindow();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("BlueBrick")) {


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Bricks")) {


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window")) {
				if (ImGui::MenuItem("Fullscreen", "F11", &fullscreen))
					ToggleFullscreen();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		ImGui::EndFrame();
		ImGui::Render();
	}

}
