#pragma once

#include "ofAppRunner.h"
#include "ofGraphics.h"

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>
#include <VW_framework/3D/Overlays/reference_grid.h>

/*
					SCENE VIEWPORT PROPOERTIES WIDGET

	A widget class that displays scene viewport extensions and modifications
	that define what is to be displayed in the viewport and on the computer
	screen.
*/

class viewport_properties_widget_class {
public:
	viewport_properties_widget_class() {}
	~viewport_properties_widget_class() {}

	reference_grid_class *reference_grid = nullptr;

	ImVec4 xy_grid_color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 xz_grid_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	ImVec4 yz_grid_color = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);

	ImVec4 xhair_color   = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	bool *display_crosshairs  = nullptr;
	bool *display_camera_info = nullptr;
	bool *display_gimbal      = nullptr;

	ofColor* xhair_color_viewport = nullptr;

	void display() {
		float x_pos = 10.0f, y_pos = 45.0f;

		title("Scene Viewport");

		if (reference_grid) {
			y_pos += 25;
			if (ImGui::CollapsingHeader("Grid###vgridch")) {
				y_pos += 25;
				text("Grid Plane   X-Y       X-Z       y-Z", x_pos, y_pos);
				y_pos += 25;
				text("Display:", x_pos, y_pos);
				y_pos += 25;
				ImGui::SetCursorPosX(x_pos + 103); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbdxy", &reference_grid->display_xy_plane);
				ImGui::SetCursorPosX(x_pos + 180); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbdxz", &reference_grid->display_xz_plane);
				ImGui::SetCursorPosX(x_pos + 257); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbdyz", &reference_grid->display_yz_plane);

				y_pos += 25;
				text("Relative:", x_pos, y_pos);
				ImGui::SetCursorPosX(x_pos + 103); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbrxy", &reference_grid->relative_xy_grid);
				ImGui::SetCursorPosX(x_pos + 180); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbrxz", &reference_grid->relative_xz_grid);
				ImGui::SetCursorPosX(x_pos + 257); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbryz", &reference_grid->relative_yz_grid);

				y_pos += 25;
				text("Relative\nlocation", x_pos, y_pos);
				y_pos += 5;
				ImGui::SetCursorPosX(x_pos + 70); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
				ImGui::DragFloat("###vgriddfxy", &reference_grid->relative_xy_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
				ImGui::SetCursorPosX(x_pos + 150); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
				ImGui::DragFloat("###vgriddfxz", &reference_grid->relative_xz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
				ImGui::SetCursorPosX(x_pos + 230); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
				ImGui::DragFloat("###vgriddfyz", &reference_grid->relative_yz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

				y_pos += 25;
				text("Relative to\nAbsolute", x_pos, y_pos);
				ImGui::SetCursorPosX(x_pos + 103); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbarxy", &reference_grid->relative_xy_grid_absolute);
				ImGui::SetCursorPosX(x_pos + 180); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbarxz", &reference_grid->relative_xz_grid_absolute);
				ImGui::SetCursorPosX(x_pos + 257); ImGui::SetCursorPosY(y_pos);
				ImGui::Checkbox("###vgridcbaryz", &reference_grid->relative_yz_grid_absolute);

				y_pos += 30;
				text("Absolute\nlocation", x_pos, y_pos);
				y_pos += 5;
				ImGui::SetCursorPosX(x_pos + 70); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
				ImGui::DragFloat("###vgriddfaxy", &reference_grid->relative_xy_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
				ImGui::SetCursorPosX(x_pos + 150); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
				ImGui::DragFloat("###vgriddfaxz", &reference_grid->relative_xz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);
				ImGui::SetCursorPosX(x_pos + 230); ImGui::SetCursorPosY(y_pos); ImGui::SetNextItemWidth(75);
				ImGui::DragFloat("###vgriddfayz", &reference_grid->relative_yz_grid_dist, 0.005f, -FLT_MAX, +FLT_MAX, "%.3f", ImGuiSliderFlags_None);

				y_pos += 35;
				text("Color", x_pos, y_pos);
				ImGui::PushStyleColor(ImGuiCol_Button, xy_grid_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
				if (ex_button("X-Y###plcxy", x_pos + 70, y_pos, 75, 20)) {
					ImGui::OpenPopup("X-Y###plcpuxy");
				}
				ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

				if (ImGui::BeginPopup("X-Y###plcpuxy")) {
					ImGuiColorEditFlags flags = misc_flags;
					flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
					ImGui::ColorPicker4("X-Y###cpxy", (float*)&xy_grid_color, flags, ref_color ? &ref_color_v.x : NULL);
					reference_grid->ofxy_grid_color.r = int(xy_grid_color.x * 255.0);
					reference_grid->ofxy_grid_color.g = int(xy_grid_color.y * 255.0);
					reference_grid->ofxy_grid_color.b = int(xy_grid_color.z * 255.0);
					reference_grid->ofxy_grid_color.a = int(xy_grid_color.w * 255.0);
					//scene_viewer->viewer_grid.update_viewer_grid_colors(XY_PLANE_GRID_COLOR);
					ImGui::EndPopup();
				}

				ImGui::PushStyleColor(ImGuiCol_Button, xz_grid_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
				if (ex_button("X-Z###plcxz", x_pos + 150, y_pos, 75, 20)) {
					ImGui::OpenPopup("X-Z###plcpuxz");
				}
				ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

				if (ImGui::BeginPopup("X-Z###plcpuxz")) {
					ImGuiColorEditFlags flags = misc_flags;
					flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
					ImGui::ColorPicker4("X-Z###cpxz", (float*)&xz_grid_color, flags, ref_color ? &ref_color_v.x : NULL);
					reference_grid->ofxz_grid_color.r = int(xz_grid_color.x * 255.0);
					reference_grid->ofxz_grid_color.g = int(xz_grid_color.y * 255.0);
					reference_grid->ofxz_grid_color.b = int(xz_grid_color.z * 255.0);
					reference_grid->ofxz_grid_color.a = int(xz_grid_color.w * 255.0);
					//scene_viewer->viewer_grid.update_viewer_grid_colors(XZ_PLANE_GRID_COLOR);
					ImGui::EndPopup();
				}

				ImGui::PushStyleColor(ImGuiCol_Button, yz_grid_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
				if (ex_button("Y-Z###plcyz", x_pos + 230, y_pos, 75, 20)) {
					ImGui::OpenPopup("Y-Z###plcpuyz");
				}
				ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

				if (ImGui::BeginPopup("Y-Z###plcpuyz")) {
					ImGuiColorEditFlags flags = misc_flags;
					flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
					ImGui::ColorPicker4("Y-Z###cpyz", (float*)&yz_grid_color, flags, ref_color ? &ref_color_v.x : NULL);
					reference_grid->ofyz_grid_color.r = int(yz_grid_color.x * 255.0);
					reference_grid->ofyz_grid_color.g = int(yz_grid_color.y * 255.0);
					reference_grid->ofyz_grid_color.b = int(yz_grid_color.z * 255.0);
					reference_grid->ofyz_grid_color.a = int(yz_grid_color.w * 255.0);
					//scene_viewer->viewer_grid.update_viewer_grid_colors(YZ_PLANE_GRID_COLOR);
					ImGui::EndPopup();
				}

			}
		}

		if (display_crosshairs) {
			y_pos += 40;
			checkbox("###vxhair", "X Hair", x_pos+100, y_pos, 100, 0, *display_crosshairs);
			//checkbox("###vxhair", "X Hair", x_pos, y_pos, 100, 0, *display_crosshairs, imgui_text_location_enum::left);
			ImGui::PushStyleColor(ImGuiCol_Button, xhair_color);// Chanage all the buttons to the grid color until a ImGui::PopStyleColor
			if (ex_button("###plcxh", x_pos + 170, y_pos, 75, 20)) {
				ImGui::OpenPopup("X-Hairs###plcpuxh");
			}
			ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

			if (ImGui::BeginPopup("X-Hairs###plcpuxh")) {
				ImGuiColorEditFlags flags = misc_flags;
				flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
				ImGui::ColorPicker4("X-Hairs###cpxh", (float*)&xhair_color, flags, ref_color ? &ref_color_v.x : NULL);
				xhair_color_viewport->r = (int)(xhair_color.x*255.0f);
				xhair_color_viewport->g = (int)(xhair_color.y*255.0f);
				xhair_color_viewport->b = (int)(xhair_color.z*255.0f);
				xhair_color_viewport->a = (int)(xhair_color.w*255.0f);
				ImGui::EndPopup();
			}
		}
		
		//x_pos += 100;
		if (display_camera_info) {
			y_pos += 25;
			checkbox("###vcaminfo", "Camera Info", x_pos+100, y_pos, 100, 0, *display_camera_info);
			//checkbox("###vcaminfo", "Camera Info", x_pos, y_pos, 0, 0, *display_camera_info, imgui_text_location_enum::left);
		}

		if (display_gimbal) {
			y_pos += 25;
			checkbox("###gimbal", "Gimbal", x_pos+100, y_pos, 100, 0, *display_gimbal);
			//checkbox("###gimbal", "Gimbal", x_pos, y_pos, 0, 0, *display_gimbal, imgui_text_location_enum::left);
		}

		y_pos += 50;
		x_pos -= 20;
		text("Viewer Background Color",x_pos+100,y_pos);

		y_pos += 25;
		x_pos -= 20;

		ImGui::SetCursorPosX(x_pos+100);
		ImGui::SetCursorPosY(y_pos);
		ImGuiColorEditFlags flags = misc_flags;
		flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
		//if(openGL_context != NULL) 
		//	ImGui::ColorPicker4("###vbgc", (float*)&openGL_context->background_color, flags, ref_color ? &ref_color_v.x : NULL);
		
		// ++++++++++
		if (ofGetCurrentRenderer()) {
			background_color = ofGetBackgroundColor();
			ImColor im_color(background_color.r, background_color.g, background_color.b,background_color.a);
			if (ImGui::ColorPicker4("###vbgc", (float*)&im_color, flags, ref_color ? &ref_color_v.x : NULL)) {
				//openframeworks only uses intergers to represent rgba color method !!!!!!
				background_color.r = (int)(im_color.Value.x * 255.0f);
				background_color.g = (int)(im_color.Value.y * 255.0f);
				background_color.b = (int)(im_color.Value.z * 255.0f);
				background_color.a = (int)(im_color.Value.w * 255.0f);
				ofBackground(background_color); // set openframeworks viewer background color
			}
		}
		// ++++++++++


	}

private:

	ofColor background_color;// +++++

	//bool fps      = true;
	bool grid     = true;
	bool axis     = true;
	bool cam_info = true;
	//bool x_hair = true;

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

	//if (!alpha)            flags |= ImGuiColorEditFlags_NoAlpha;        // This is by default if you call ColorPicker3() instead of ColorPicker4()
	//if (alpha_bar)         flags |= ImGuiColorEditFlags_AlphaBar;
	//if (!side_preview)     flags |= ImGuiColorEditFlags_NoSidePreview;
	//if (picker_mode == 1)  flags |= ImGuiColorEditFlags_PickerHueBar;
	//if (picker_mode == 2)  flags |= ImGuiColorEditFlags_PickerHueWheel;
	//if (display_mode == 1) flags |= ImGuiColorEditFlags_NoInputs;       // Disable all RGB/HSV/Hex displays
	//if (display_mode == 2) flags |= ImGuiColorEditFlags_DisplayRGB;     // Override display mode
	//if (display_mode == 3) flags |= ImGuiColorEditFlags_DisplayHSV;
	//if (display_mode == 4) flags |= ImGuiColorEditFlags_DisplayHex;
	// **************************************************

};