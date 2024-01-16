#pragma once
#include <string>
#include <vector>

#include "timeline_parameters.h"

#include <FrameWork/Universal_FW/Animation/animation.h>

template <class T>
struct timeline_interval_data_struct {
	//void* data_link; // probably not needed but here to remind that this needs to also be linked to data that the interval track modifies or reads
	
	unsigned int id; // unique id for this interval so that it can be indetified and used by ImGui for its widget id assignment
	//std::string  label; // probably not needed

	T interval_min_frame, interval_max_frame;
	//std::pair(T ,T) interval;

	ImColor fill_color;

};

struct timeline_track_style_struct {
	ImColor frame_fill_color    = IM_COL32(25, 25, 25, 255);
	ImColor current_frame_color = IM_COL32(0, 255, 0, 255);
	// Other parameter data here as needed

};

class timeline_int_interval_class : public timeline_track_basis_class {
public:
	//int id = -1; // not sure will be needed
	int button_id_add = 0;

	std::string label = "track";

	float timeline_interval_min_frame = 9.0f;  // testing assign to zero when finished
	float timeline_interval_max_frame = 10.0f;

	animation_object_basis_class *animation_object = nullptr; // animation object this interval track is linked to +++++++++++++++

	// At the moment this is set up only for a single interval track
	// In future need to cater for multiple intervals per track
	// ie have std::vector<int_timeline_parameter_data_struct_type> 
	// as parmater passed to functions in place of 
	// int_timeline_parameter_data_struct_type
	std::vector<timeline_interval_data_struct<int>> intervals;

	void display_track(float track_width, int_timeline_parameter_data_struct_type timeline_data) {
		timeline_int_interval_track(track_width,timeline_data);
		ImVec2 sc_pos = ImGui::GetCursorScreenPos();
		display_track_activity_widget({ track_width + 95.0f,23.0f });
	}

	bool timeline_int_interval_track(float widget_width, int_timeline_parameter_data_struct_type timeline_data);

	bool perform_interval_behavior(ImGuiWindow* window, 
								   float widget_width,
								   int_timeline_parameter_data_struct_type timeline_data);

	void draw_timeline_interval(ImGuiWindow* window, 
                                float widget_width,
                                int_timeline_parameter_data_struct_type timeline_data,
                                float min_button_pos,float max_button_pos);

	// -------------------
	// following used until all tack intervals are stored within intervals
	bool frame_in_interval(int frame) {
		if (frame >= current_min_frame && frame <= current_max_frame)
			return true;

		return false;
	}

	// This is the goal of having more than one interval per track but is not implemented yet
//	bool frame_in_interval(int frame) {
//		for (timeline_interval_data_struct<int> interval : intervals) {
//std::cout << "timeline_int_interval_class :: frame_in_interval : " << frame << ":" << interval.interval_min_frame << ":" << interval.interval_max_frame << std::endl;
//			if (frame >= interval.interval_min_frame && frame <= interval.interval_max_frame)
//				return true;
//		}
//		return false;
//	}

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

	int   current_min_frame = 9;
	int   current_max_frame = 10;

	float prev_min_button_pos    = 0.0f;
	float current_min_button_pos = 0.0f;
	float prev_max_button_pos    = 0.0f;
	float current_max_button_pos = 0.0f;

	bool  min_frame_button_active = false;
	bool  min_button_initial      = false;
	bool  max_frame_button_active = false;
	bool  max_button_initial	  = false;

	ImVec2 sc_pos;

	int   no_frames;
	float frames_per_pixel;
	float pixels_per_frame;

	//bool value_changed = false;

	//bool timeline_interval_max_track_widget(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
	//bool timeline_interval_track_widget(float width, timeline_parameter_data_struct_type timeline_data);
};