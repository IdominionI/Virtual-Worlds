#pragma once
#include <string>
#include <vector>

#include "imgui.h"
#include "imgui_internal.h"

class imgui_int_range_widget_class {
public:
	//imgui_int_range_widget_class(int min,int max):
	//	min_value(min),
	//	max_value(max),
	//	current_min_frame(min),
	//    current_max_frame(max)
	//{}

	std::string label = "track";
	ImVec2 offset = { 0,0 };

	int min_value = 0.0f;
	int max_value = 255.0f;

	ImVec2 min_text_offset = { 0.0,0.0 };
	ImVec2 max_text_offset = { 0.0,0.0 };

	int current_min_frame = 0;
	int current_max_frame = 255;

	bool display_int_range_widget(float track_width,int min,int max) {
		if (!set) { // this is needed as use a constructor only works if this class is created as a new imgui_int_range_widget_class
			min_value = min;
			max_value = max;
			current_min_frame = min;
			current_max_frame = max;
			set = true;
		}
		ImVec2 sc_pos = ImGui::GetCursorScreenPos();
		return range_int_interval_widget(track_width);
	}

private:
	bool set = false;
	int button_id_add = 0;

	float prev_min_frame = 0.0f;
	float prev_max_frame = 255.0f;

	float prev_min_button_pos    = 0.0f;
	float current_min_button_pos = 0.0f;
	float prev_max_button_pos    = 0.0f;
	float current_max_button_pos = 0.0f;

	bool  min_button_initial      = false;
	bool  max_button_initial	  = false;

	ImVec2 sc_pos = { 0,0 };

	int   no_frames = 0;
	float frames_per_pixel = 0.0;
	float pixels_per_frame = 0.0 ;

	bool range_int_interval_widget(float widget_width);

	bool perform_range_behavior(ImGuiWindow* window, float widget_width);

	void draw_range_interval(ImGuiWindow* window, 
                             float widget_width,
                             float min_button_pos,float max_button_pos);

};