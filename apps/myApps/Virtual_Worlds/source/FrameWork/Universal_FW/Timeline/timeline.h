#pragma once

//#include "timeline_paramters.h" // Include this if none of the lower included files has it define

//#include "timeline_interval.h"

#include "timeline_tracks.h"

class timeline_class{
public:
	int_timeline_parameter_data_struct_type timeline_data;
	
	bool display_int_slider_value = false; 
	bool display_int_slider_frame = true;

	// timeline_zoom_widget is a modified copy of the ImGui SliderScalar widget.
	bool timeline_integer_zoom_widget(const char* label, ImGuiDataType data_type, float cursor_x, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
	
	void draw_timeline_frame_annotation();

	// timeline_integer_slider and timeline_integer_slider_widget are a modified copy of the ImGui SliderFloat & SliderScalar widgets.
	// For some strange reason could not use a modified SliderScalar code for float on it own without being referenced from timeline_integer_slider
	// as the numbers returned were very large.
	bool timeline_integer_slider(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags) {
		return timeline_integer_slider_widget(label, ImGuiDataType_Float, v, &v_min, &v_max, format, flags);
	}

	// ++++++++++++++++++++++
	timeline_tracks_class timeline_tracks_widget;
	// ++++++++++++++++++++++

private :
	bool timeline_integer_slider_widget(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);


};
