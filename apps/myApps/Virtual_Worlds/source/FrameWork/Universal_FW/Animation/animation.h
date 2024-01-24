#pragma once
#include <vector>

#include <iostream> // Testing only : delete comment out when complete

typedef std::pair<int, int>     animation_frame_interval_type;
typedef std::pair<float, float> animation_time_interval_type;

enum class animation_action_enum { none, step_forward, step_back, play, reverse_play, stop, pause, record,cycle };

struct animation_data_struct_type {
	animation_action_enum animation_action = animation_action_enum::none;
	int animation_action_extra = 0;

	int   current_frame = 0;
	float current_time  = 0.0f;

	int   previous_frame = 0;
	float previous_time  = 0.0f;

	float frames_per_second = 24.0f;
	float seconds_per_frame = 1.0f/24.0f;

	int   frame_step   = 1;
	float time_step    = 1.0f/24.0f;

	void update(int frame) {
		previous_frame = current_frame;
		previous_time  = current_time;
		current_frame  = frame;
		current_time = calc_current_time();
	}

	float calc_current_time() {
		return time_duration_from_frames(current_frame);
	}

	int calc_current_frame() {
		return no_frames_in_timeI(current_time);
	}

	float time_duration_from_frames(int number_frames) {
		return float(number_frames) * frames_per_second;
	}

	float no_frames_in_timeR(float time) {
		return abs(frames_per_second/time);
	}

	int no_frames_in_timeI(float time) {
		return int(floor(no_frames_in_timeR(time)));
	}

	int frame_step_interval() {
		return current_frame - previous_frame;
	}

	float time_step_interval() {
//std::cout << "animation_data_struct_type:time_step_interval : 0000 : " << current_frame << ":" << previous_frame << ":" << seconds_per_frame << "::" << float(current_frame - previous_frame) * seconds_per_frame << std::endl;
		return float(current_frame - previous_frame)* seconds_per_frame;
	}
};

enum class animation_keyframe_interpolation_type_enum {step,linear}; // others to be added later

class animation_keyframe_basis_class {
public:
	//int interval_id = -1; //not sure will be needed

	// Keyframe data and methods asocitate with that data are defined in the derived
	// object keyframe data class that is then used in the animation pipeline

	bool keyframe_active = true;

	animation_keyframe_interpolation_type_enum interpolation_type;

	class animation_keyframe_basis_class *prev_keyframe = nullptr;
	class animation_keyframe_basis_class *next_keyframe = nullptr;

	int   frame;
	float time;

	virtual void display_keyframe_data_to_edit() {}
	virtual void perform_keframe_animation(animation_data_struct_type animation_data) {}
	virtual void upadate_keframe_animation_data(animation_data_struct_type animation_data) {}

private:
};


struct animation_interval_struct {
public:
	//int interval_id = -1; not sure will be needed

	bool interval_active = true;

	animation_frame_interval_type frame_interval;
	animation_time_interval_type  time_interval;
	
	void update_time_interval(animation_data_struct_type animation_data) { // called only when frame interval is changed externally
		time_interval.first  = animation_data.time_duration_from_frames(frame_interval.first);
		time_interval.second = animation_data.time_duration_from_frames(frame_interval.second);
	}

	void update_frame_interval(animation_data_struct_type animation_data) { // called only when time interval is changed externally
		frame_interval.first  = animation_data.no_frames_in_timeI(time_interval.first);
		frame_interval.second = animation_data.no_frames_in_timeI(time_interval.second);
	}

	bool frame_inside_interval(int frame) {
		return(frame >= frame_interval.first && frame <= frame_interval.second);
	}

	bool time_inside_interval(float time) {
		return(time >= time_interval.first && time <= time_interval.second);
	}

private:
};




class animation_object_basis_class {
public:
	// Individual objcet data types placed here
	//keyframe_data_structure_type keyframe_data; // to be defined by the class that inhehits this basis class
	//object_data_type             initial_parameter_settings;// to be defined by the class that inhehits this basis class

	bool animation_reversable = false;

	std::vector<animation_interval_struct>      intervals;
	std::vector<animation_keyframe_basis_class> keyframes;

	virtual bool setup_animation_data(animation_data_struct_type animation_settings) { return true; }
	virtual bool update_animation_data(animation_data_struct_type animation_settings) { return true; }
	//virtual void perform_animation_for_frame(int frame) {}
	virtual void perform_animation_for_frame(int frame, int track) {}
	//virtual void perform_animation_for_time(float time) {}
	virtual void perform_animation_for_time(float time, int track) {}
	virtual void restore_initial_frame() {} // A function that is defined in the derived animation object class using its unique data

	bool have_active_frame() {
		for (animation_interval_struct interval : intervals) {
			if (interval.interval_active) return true;
		}
		return false;
	}

	bool frame_is_in_active_interval(int frame) {
		for (animation_interval_struct interval : intervals) {
			if (interval.frame_inside_interval(frame) && interval.interval_active)
				return true;
		}
	}

	//bool time_is_in_active_interval(int time) {
	//	for (animation_interval_struct interval : intervals) {
	//		if (interval.time_inside_interval(time) && interval.interval_active)
	//			return true;
	//	}
	//}

	// Individual object animation tasks are placed here

private:
};
