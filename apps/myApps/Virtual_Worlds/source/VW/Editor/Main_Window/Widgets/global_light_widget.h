#pragma once

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

#include <ofLight.h>


class global_light_parameters_widget_class {
public:

	ImVec4 light_color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 255.0f / 255.0f);

	ofLight *global_light = nullptr;

	bool display() {
		if (!global_light) {
			return false;
		}

		float x_pos = 10.0f, y_pos = 40.0f;

		//title("Global Lighting");

		y_pos += 40;

		ImGui::PushStyleColor(ImGuiCol_Button, light_color);// Chanage all the buttons to the light color until a ImGui::PopStyleColor
		if (ex_button("Light Color###plc", x_pos + 130, y_pos, 100, 30)) {
			ImGui::OpenPopup("Light Color###plcpu");
		}
		ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

		if(	ImGui::BeginPopup("Light Color###plcpu")) {
			ImGuiColorEditFlags flags = misc_flags;
			flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
			ImGui::ColorPicker4("Light Color###cp", (float*)&light_color, flags, ref_color ? &ref_color_v.x : NULL);

			global_light->setAmbientColor({ light_color.x,light_color.y,light_color.z,light_color.w });

			ImGui::EndPopup();
		}

		y_pos +=40;
		text(" Global Lighting\n   Orientation", x_pos + 123,y_pos);
		y_pos += 35;
		text("Pitch           Yaw             Roll", x_pos + 50, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos+5);
		ImGui::SetCursorPosY(y_pos);
		if (ImGui::SliderFloat3("###lpdv", lighting_dir_vector, -180.0f, 180.0f, "%.2f")) {
			global_light->setOrientation(glm::vec3{ lighting_dir_vector[0],lighting_dir_vector[1],lighting_dir_vector[2]});
		}

		y_pos += 30;
		text("Global Light Intesity", x_pos + 105, y_pos);
		y_pos += 20;
		ImGui::SetCursorPosX(x_pos+140);
		ImGui::SetCursorPosY(y_pos);
		ImGui::SetNextItemWidth(100);
		if (ImGui::SliderFloat("###plal", &intensity, 0.0f, 1.0f, "%.2f")) {
			global_light->intensity = intensity;
		}

		return true;
	}


private:
	float lighting_dir_vector[3] = {0.0f,0.0f,0.0f};
	float intensity = 0.75;

	// ********** Color Picker definitions *****************
	bool alpha_preview      = true;
	bool alpha_half_preview = false;
	bool drag_and_drop      = true;
	bool options_menu       = true;
	bool hdr                = false;

	ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

	//ImGui::Text("Color picker:");
	bool alpha         = true;
	bool alpha_bar     = true;
	bool side_preview  = true;
	bool ref_color     = false;
	ImVec4 ref_color_v = { 1.0f, 0.0f, 1.0f, 0.5f };
	int display_mode   = 0;
	int picker_mode    = 0;
};