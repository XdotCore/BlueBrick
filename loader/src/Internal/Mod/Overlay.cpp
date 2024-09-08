#include "Mod/Overlay.hpp"
#include "Overlay.hpp" // The platform specific one
#include "imgui.h"
#include <thread>

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
		
		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = "BlueBrick/imgui.ini";

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		// TODO: make this not not stack overflow lmao
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		// Regular [0]
		io.Fonts->AddFontFromFileTTF("BlueBrick/fonts/CascadiaCode/CascadiaCode.ttf", 16.f);
	}

	void Overlay::Draw() {
		ImGui::NewFrame();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("BlueBrick")) {
				ImGui::MenuItem("Show Logs", nullptr, &showLogs);

				ImGui::MenuItem("Show ImGui Demo", nullptr, &showDemoWindow);

				ImGui::Separator();

				if (ImGui::MenuItem("Quit", "Alt + F4"))
					std::exit(0);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Bricks")) {
				if (ImGui::MenuItem("Hello")) {

				}

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (showLogs)
			ShowLogs();

		if (showDemoWindow)
			ImGui::ShowDemoWindow(&showDemoWindow);

		ImGui::EndFrame();
		ImGui::Render();
	}

	void Overlay::PostDraw() {
		static ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void Overlay::ShowLogs() {
		ImGui::SetNextWindowSize(ImVec2(700, 650), ImGuiCond_FirstUseEver);
		ImGui::Begin("Log Window", &showLogs, ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 2));

		bool colored = false;
		for (const LogItemTypes& item : logItems) {
			// text and newlines
			if (std::holds_alternative<std::string>(item)) {
				const std::string& string = std::get<std::string>(item);
				if (string == "\n")
					ImGui::TextUnformatted("");
				else {
					ImGui::TextUnformatted(string.c_str());
					ImGui::SameLine();
				}
			}
			// add color
			else if (std::holds_alternative<Color>(item)) {
				const Color& color = std::get<Color>(item);

				if (colored)
					ImGui::PopStyleColor();

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.rf, color.gf, color.bf, 1));
				colored = true;
			}
			// remove color
			else if (std::holds_alternative<ColorNone>(item)) {
				if (colored)
					ImGui::PopStyleColor();

				colored = false;
			}
		}

		if (colored)
			ImGui::PopStyleColor();

		ImGui::PopStyleVar();

		// scroll to end, including logic from the scrollbar covering part of the window
		if (logChanged && (ImGui::GetScrollY() + ImGui::GetStyle().ScrollbarSize) >= ImGui::GetScrollMaxY()) {
			ImGui::SetScrollHereY(1);
			logChanged = false;
		}

		ImGui::End();
	}

	void Overlay::AddLogItems(const std::vector<LogItemTypes>& items) {
		logItems.insert(logItems.end(), items.begin(), items.end());
		logChanged = true;
	}

}
