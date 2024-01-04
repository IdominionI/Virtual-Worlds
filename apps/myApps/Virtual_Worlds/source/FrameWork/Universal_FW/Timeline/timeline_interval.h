#pragma once
#include <string>

#include "timeline_parameters.h"

class timeline_int_interval_class : public timeline_track_basis_class {
public:
	//int id = -1; // not sure will be needed
	int button_id_add = 0;

	std::string label = "track";

	float timeline_interval_min_frame = 9.0f;
	float timeline_interval_max_frame = 10.0f;

	void display_track(float track_width, int_timeline_parameter_data_struct_type timeline_data) {
		timeline_int_interval_track(track_width,timeline_data);
		ImVec2 sc_pos = ImGui::GetCursorScreenPos();
		display_track_activity_widget({ track_width + 95.0f,23.0f });
	}

	bool timeline_int_interval_track(float widget_width, int_timeline_parameter_data_struct_type timeline_data);

	// -------------------

	bool interval_below_display_range(int_timeline_parameter_data_struct_type timeline_data) {
		return (timeline_data.below_timeline_display_range(timeline_interval_min_frame) && timeline_data.below_timeline_display_range(timeline_interval_max_frame));
	}
	
	bool interval_above_display_range(int_timeline_parameter_data_struct_type timeline_data) {
		return (timeline_data.above_timeline_display_range(timeline_interval_min_frame) && timeline_data.above_timeline_display_range(timeline_interval_max_frame));
	}
	
	bool interval_in_display_range(int_timeline_parameter_data_struct_type timeline_data) {
		return (!interval_below_display_range(timeline_data) && !interval_above_display_range(timeline_data));
	}

	bool interval_below_range(int_timeline_parameter_data_struct_type timeline_data) {
		return (timeline_data.below_timeline_range(timeline_interval_min_frame) && timeline_data.below_timeline_range(timeline_interval_max_frame));
	}

	bool interval_above_range(int_timeline_parameter_data_struct_type timeline_data) {
		return (timeline_data.above_timeline_range(timeline_interval_min_frame) && timeline_data.above_timeline_range(timeline_interval_max_frame));
	}

	bool interval_in_range(int_timeline_parameter_data_struct_type timeline_data) {
		return (!interval_below_range(timeline_data) && !interval_above_range(timeline_data));
	}

	// -------------------
	
protected:

private:
	float timeline_interval_prev_min_frame = 9.0f;
	float timeline_interval_prev_max_frame = 10.0f;

	int   current_min_frame = 0;
	int   current_max_frame = 0;

	float prev_min_button_pos    = 0.0f;
	float current_min_button_pos = 0.0f;
	float prev_max_button_pos    = 0.0f;
	float current_max_button_pos = 0.0f;

	bool  min_frame_button_active = false;
	bool  min_button_initial      = false;
	bool  max_frame_button_active = false;
	bool  max_button_initial	  = false;

	//bool timeline_interval_max_track_widget(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
	//bool timeline_interval_track_widget(float width, timeline_parameter_data_struct_type timeline_data);
};