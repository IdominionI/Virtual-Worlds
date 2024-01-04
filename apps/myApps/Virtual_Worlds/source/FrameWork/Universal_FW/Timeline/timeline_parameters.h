#pragma once

#include <FrameWork/Kernels/imgui-docking/imgui.h>
#include <FrameWork/Kernels/imgui-docking/imgui_internal.h>

struct int_timeline_parameter_data_struct_type {
	float zoom             = 5.0f;
	int zoom_current_frame = 8;// Initial value must be same as start frame
	int zoom_start_frame   = 8;// Initial value must be same as start frame
	int zoom_end_frame     = 0;

	int current_frame = 10;
	int start_frame   = 8;
	int end_frame     = 24;

	float current_frame_scroll = 0.5f;
	int   frame_step_increment = 1;

	ImVec2 wsize = { 500.0f, 500.0f }; // timeline window size
	ImVec2 annotation_wsize = { 500.0f, 50.0f }; // timeline annotation widget window size
	float  window_x_space = 150.0f;
	ImVec2 input_box_size = {60.0f,15.0f};


	bool inside_timeline_display_range(int frame) {
		return (frame >= zoom_start_frame && frame <= zoom_end_frame);
	}

	bool below_timeline_display_range(int frame) {
		return (frame < zoom_start_frame);
	}

	bool above_timeline_display_range(int frame) {
		return (frame > zoom_end_frame);
	}

	bool inside_timeline_range(int frame) {
		return (frame >= start_frame && frame <= end_frame);
	}

	bool below_timeline_range(int frame) {
		return (frame < start_frame);
	}

	bool above_timeline_range(int frame) {
		return (frame > end_frame);
	}
};

class timeline_track_basis_class {
public:
	int track_id = 0;
	std::string track_name = "Track";
	int track_type_id = -1;

	void* data_ptr = nullptr;
	int  data_type_id = -1;

	bool track_selected = false;
	bool track_active   = false;

	virtual void display_track(float track_width, int_timeline_parameter_data_struct_type timeline_data) {};

	virtual void display_track_activity_widget(ImVec2 location) {
		ImVec2 loc = ImGui::GetCursorPos();
		ImGui::SetCursorPosX(location.x);
		ImGui::SetCursorPosY(loc.y-location.y);
		std::string checkbox_id = "###tlt0" + std::to_string(track_id);
		ImGui::Checkbox(checkbox_id.c_str(), &track_active);
		ImGui::SetCursorPos(loc);
	}

	virtual void create_track(float track_width, int_timeline_parameter_data_struct_type timeline_data) {};
	virtual void delete_track(float track_width, int_timeline_parameter_data_struct_type timeline_data) {};


private:
};