#pragma once

#include <Kernels/imgui-docking/imgui.h>

#include <Universal_FW/animation/animation.h>
#include <Universal_FW/Timeline/timeline.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>
#include "../Widgets/parameters_widget.h"

class animation_timeline_panel_class : public timeline_class {
public:
    animation_data_struct_type animation_settings;
    animation_interval_struct  animation_range;

    std::vector<animation_object_basis_class> animation_objects;


    bool is_first_frame(int frame) {
        return (frame == animation_range.frame_interval.first);
    }
    bool is_last_frame(int frame) {
        return (frame == animation_range.frame_interval.second);
    }

    // Following needs testing for round off errors
    bool is_first_frame(float time) {
        int frame = int(time * animation_settings.frames_per_second);
        return (is_first_frame(frame));
    }
    bool is_last_frame(float time) {
        int frame = int(time * animation_settings.frames_per_second);
        return (is_last_frame(frame));
    }

    void show() {
        float x_pos = 0.0f, y_pos = 100.0f;

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        timeline_data.wsize = window->Size;
//std::cout << "animation_timeline_panel_class : show : current sequencer : " << timeline_data.wsize.x << ":" << timeline_data.wsize.y << std::endl;

        ImGui::Begin("Animation-timeline");// Remove/comment out this if do not want dockable panel
       
        display_timeline_controls();

        // Diaplsy frame range zoom slider 
        integer_input("###tlsf", timeline_data.start_frame, x_pos + 10, y_pos, timeline_data.input_box_size.x);

        ImGui::SetCursorPosX(x_pos + 75);
        ImGui::SetCursorPosY(y_pos);
        timeline_integer_zoom_widget("zoomscrollbar", ImGuiDataType_U32, ImGui::GetCursorPosX(), &timeline_data.zoom_current_frame, &timeline_data.start_frame, &timeline_data.end_frame, "%d", 0);
        integer_input("###tlef", timeline_data.end_frame, window->Size.x - 70, y_pos, timeline_data.input_box_size.x);

        // Display current frame slider
        ImGui::SetCursorPosX(x_pos + 75);
        ImGui::SetNextItemWidth(window->Size.x - timeline_data.window_x_space);
        //ImGui::SliderFloat("###tfsb", &timeline_data.current_frame_scroll, 0.0f, 1.0f, "%f", 0); for testing only
        ImGui::SetCursorPosX(x_pos + 75);
        //display_int_slider_frame = false;
        timeline_integer_slider("###tfsb2", &timeline_data.current_frame_scroll, 0.0f, 1.0f, "%f", 0);

        // Display zoom frame range 
        ImGui::SetCursorPosX(x_pos + 75 - 7);// set location for timeline annotation widget that lines up with timeline zoom scrolling widget
        int i = 1;
        float child_height = ImGui::GetTextLineHeight() + 20;
        ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar;
        ImGuiID child_id = ImGui::GetID((void*)(intptr_t)i);

        ImGui::BeginChild(child_id, ImVec2(window->Size.x - timeline_data.window_x_space + 14, child_height), true);
            draw_timeline_frame_annotation();
        ImGui::EndChild();

        ImGui::SetCursorPosX(5);
        timeline_tracks_widget.display_tracks(ImVec2(window->Size.x - 10, window->Size.y - 195), window->Size.x - timeline_data.window_x_space, timeline_data);

		perforn_animation();

        ImGui::End;// Remove/comment out this if do not want dockable panel

	}

    void display_timeline_controls() {
        float x_pos = timeline_data.wsize.x / 2.0 - 85;
        float y_pos = 0.0f;

        //float x_pos = x_pos + 90;
        text("Animation Controls", x_pos + 20, y_pos);
        y_pos += 20;

        if (ex_button(ICON_FA_CARET_LEFT "###vaupbb", x_pos, y_pos, 40, 23))
            reverse_animation();

        if (ex_button(ICON_FA_STOP "###vausb", x_pos + 43, y_pos, 40, 23))
            stop_animation();

        if (ex_button(ICON_FA_CIRCLE "###vaurb", x_pos + 86, y_pos, 40, 23))//ICON_FA_CIRCLE - ICON_FA_RECORD_VINAL
            record_animation();

        if (ex_button(ICON_FA_PLAY "###vaupfb", x_pos + 129, y_pos, 40, 23))
            play_animation();

        y_pos += 30;

        if (ex_button(ICON_FA_STEP_BACKWARD "###vausbb", x_pos, y_pos, 40, 23))
			perform_animation_backward_step();

        if (ex_button(ICON_FA_PAUSE "###vauppb", x_pos + 43, y_pos, 40, 23))
            pause_animation();

        if (ex_button(ICON_FA_RETWEET "###vaurpb", x_pos + 86, y_pos, 40, 23))
            cycle_animation();

        if (ex_button(ICON_FA_STEP_FORWARD "###vausfb", x_pos + 129, y_pos, 40, 23))
			perform_animation_forward_step();

        y_pos += 30;

        text("Zoom factor", x_pos - 170, y_pos);
        float_input_delta("###zf0", timeline_data.zoom, 1.0f, x_pos - 90, y_pos, 60, ICON_FA_CARET_UP"###zf0u", ICON_FA_CARET_DOWN"###zf0d");
        if (timeline_data.zoom < 1.0f) timeline_data.zoom = 1.0f;
        if(float(timeline_data.end_frame- timeline_data.start_frame)/ timeline_data.zoom < 2.0f)
            timeline_data.zoom = float(timeline_data.end_frame - timeline_data.start_frame)/2.0f;
        timeline_data.zoom = floorf(timeline_data.zoom);

        text("Current frame :", x_pos, y_pos);
        integer_input("###acf", timeline_data.current_frame, x_pos + 110, y_pos, 60);

        text("Step Increment", x_pos + 185, y_pos);
        integer_input_delta("###si0", timeline_data.frame_step_increment, 1, x_pos +290, y_pos, 60, ICON_FA_CARET_UP"###si0u", ICON_FA_CARET_DOWN"###si0d");
        if (timeline_data.frame_step_increment < 1) timeline_data.frame_step_increment = 1;
        if (timeline_data.frame_step_increment > (timeline_data.end_frame - timeline_data.start_frame)/2) timeline_data.frame_step_increment = (timeline_data.end_frame - timeline_data.start_frame) / 2;
    }

	void perforn_animation() {
		if (animation_play) {
//std::cout <<"animation_timeline_panel_class:perforn_animation 000 : animation play : " << timeline_data.current_frame << ":" << timeline_data.end_frame << std::endl;
			if (timeline_data.current_frame <= timeline_data.end_frame) {
//std::cout <<"animation_timeline_panel_class:perforn_animation AAAA : ! animation paused\n";
				if (!animation_paused) {
//std::cout <<"animation_timeline_panel_class:perforn_animation BBB : ! animation paused\n";

					setup_timeline_data();
					if (animation_range.frame_inside_interval(timeline_data.current_frame)) { //current frame is inside defined timeline boundary
						animation_settings.frame_step       = timeline_data.frame_step_increment;

						animate_current_frame(animation_play_setup);

						animation_settings.previous_frame = timeline_data.current_frame;

						if (animation_play_setup)
							animation_play_setup = false; // this needs to be set to prevent animation setup for every frame and only prform updates of data

						if(timeline_data.current_frame >= timeline_data.end_frame) // have reached the end of the time line to perform animation frame functions
							stop_animation();
						else {
							if (!animation_range.frame_inside_interval(timeline_data.current_frame + timeline_data.frame_step_increment)) {//next frame is outside defined timeline boundary
								animation_settings.current_frame = animation_range.frame_interval.second;
								timeline_data.current_frame      = animation_range.frame_interval.second;									// Assign incremented frame to max timeline boundary
							}
							else {
								animation_settings.current_frame += animation_settings.frame_step;
								timeline_data.current_frame      += timeline_data.frame_step_increment;
							}
						}
					}
				}
//std::cout <<"animation_timeline_panel_class:perforn_animation 111 : animation play\n";
				if (timeline_data.current_frame >= timeline_data.end_frame) {
//std::cout <<"animation_timeline_panel_class:perforn_animation 222 urrent_animation_frame >= end_frame\n";
					if (restore_animation_on_completion && !animation_repeat) { // Restore initial conditions of animation
						restore_initial_frame_conditions();
					} else {
//std::cout <<"animation_timeline_panel_class:perforn_animation 333\n";
						if (animation_repeat) {
//std::cout <<"animation_timeline_panel_class:perforn_animation 444\n";
							restore_initial_frame_conditions();
							animation_repeat = true;// because restore_initial_frame(); sets animation_repeat to false
							play_animation();
						}
					}
				}
			} else {
//std::cout <<"animation_timeline_panel_class:perforn_animation CCCC : ! animation stopped\n";
					stop_animation();
					timeline_data.current_frame = timeline_data.end_frame;
			}
		} else{
			if (animation_reverse_play) {
//std::cout <<"animation_timeline_panel_class:perforn_animation 000 : animation reverse play\n";
				if (timeline_data.current_frame >= timeline_data.start_frame) {
//printf("animation_reverse_play 000 ::current_animation_frame > start_frame %i : %i\n", current_animation_frame, start_frame);
					if (!animation_paused) {
//printf("animation_reverse_play 1111 ::\n");
						setup_timeline_data();
						if (animation_range.frame_inside_interval(timeline_data.current_frame)) { //current frame is inside defined timeline boundary
							animation_settings.frame_step = timeline_data.frame_step_increment;

							animate_current_frame(animation_play_setup);

							animation_settings.previous_frame = timeline_data.current_frame;

							if (animation_play_setup)
								animation_play_setup = false; // this needs to be set to prevent animation setup for every frame and only prform updates of data

							if (timeline_data.current_frame <= timeline_data.start_frame) // have reached the start of the time line to perform animation frame functions
								stop_animation();
							else {
								if (!animation_range.frame_inside_interval(timeline_data.current_frame - timeline_data.frame_step_increment)) {//next frame is outside defined timeline boundary
									animation_settings.current_frame = animation_range.frame_interval.first;
									timeline_data.current_frame      = animation_range.frame_interval.first;									// Assign decremented frame to min timeline boundary
								}
								else {
									animation_settings.current_frame -= animation_settings.frame_step;
									timeline_data.current_frame      -= timeline_data.frame_step_increment;
								}
							}
						}
					}
//else
//std::cout << "animation_timeline_panel_class:perforn_animation 000 : animation paused reverse\n"; // Not yet implemented
				} else {
//std::cout <<"animation_timeline_panel_class:perforn_animation CCCC : ! animation stopped\n";
					stop_animation();
					timeline_data.current_frame = timeline_data.start_frame;
				}
			} else{
				if (animation_record) { // Not yet implemented
//std::cout <<"animation_timeline_panel_class:perforn_animation 000 : animation record\n"; // Not yet implemented
					if (timeline_data.current_frame <= timeline_data.end_frame) {
						if (!animation_paused) {
							// animation recoed code here
							timeline_data.current_frame++;
						}
					} else {
//std::cout <<"animation_timeline_panel_class:perforn_animation CCCC : ! animation stopped\n";
						stop_animation();
						timeline_data.current_frame = timeline_data.end_frame;
					}
				}
			}
		}
	}

    // ================================================================================
		// Following assumes that animation data is correct and has been set by external code
	// that is linked to this class
	// May need to include extra functions to update data in tis class

	// this is for frame based animation

	bool have_objects_to_animate() {
		for (animation_object_basis_class animation_object : animation_objects) {
			if (animation_object.have_active_frame()) return true;
		}
		return false;
	}

	bool setup_animation() {
		bool setup_success = true;
		for (animation_object_basis_class animation_object : animation_objects) {
			if (animation_object.have_active_frame()) {
				if (!animation_object.setup_animation_data(animation_settings)) setup_success = false;
			}
		}
		return setup_success;
	}

	void reverse_animation() {
//std::cout <<"animation_timeline_panel_class:reverse_animation 000 : reverse_animation\n";
		animation_play    = false;
		animation_stopped = false;

		if (!animation_reverse_play) {
			setup_play();
			animation_reverse_play = true;
			animation_play_setup   = true;
			animation_settings.previous_frame = timeline_data.current_frame;
			animation_settings.animation_action = animation_action_enum::reverse_play;
		}

		if (animation_paused) // needs testing
			animation_play_setup = false; // Needed if user pauses play and then recomences play by pressing play

		animation_paused = false;
	}

	void play_animation() {
//std::cout <<"animation_timeline_panel_class:play_animation 000 : animation play\n";
		animation_reverse_play = false;
		animation_stopped	   = false;
		
		if (!animation_play) {
			setup_play();
			animation_play       = true;
			animation_play_setup = true;
			animation_settings.previous_frame   = timeline_data.current_frame;
			animation_settings.animation_action = animation_action_enum::play;
		}

		if(animation_paused)// needs testing
			animation_play_setup = false; // Needed if user pauses play and then recomences play by pressing play

		animation_paused = false;
	}

	void setup_timeline_data() {
		animation_settings.current_frame = timeline_data.current_frame;

		animation_range.frame_interval.first  = timeline_data.start_frame;
		animation_range.frame_interval.second = timeline_data.end_frame;
	}

	void setup_play() {
//std::cout <<"animation_timeline_panel_class:setup_play 000 : setup_play\n";
		animation_stopped = false;
		animation_paused  = false;

		setup_timeline_data();
	}

	void stop_animation() {
//std::cout <<"animation_timeline_panel_class:stop_animation 000 : stop_animation\n";
		animation_stopped      = true;
		animation_paused       = false;
		animation_play         = false;
		animation_repeat       = false;
		animation_reverse_play = false;
		animation_record       = false;

		animation_play_setup   = false;

		animation_settings.animation_action = animation_action_enum::stop;

		setup_timeline_data();
	}

	void pause_animation() {
//std::cout <<"animation_timeline_panel_class:pause_animation 000 : pause_animation\n";
		animation_paused = !animation_paused;
		//animation_settings.animation_action = animation_action_enum::pause; // Wrong to have this
	}

	// decided not to implment cycle_animation just yet : 
	// Need to reinitialise data at the end of each interval cycle and then play_animation
	// where cycle to start is the difficult decision. From current frame or start of animation range etc.
	void cycle_animation() { 
//std::cout <<"animation_timeline_panel_class:cycle_animation 000 : cycle_animation\n";
		//animation_repeat = !animation_repeat;
		//play_animation();
	}

	void record_animation() {
//std::cout <<"animation_timeline_panel_class:record_animation 000 : record_animation\n";
		// not yet decided if this is to be permitted or done in the timeline editor
	}

	bool perform_animation_forward_step() {
//std::cout <<"animation_timeline_panel_class:perform_animation_forward_step 000 : perform_animation_forward_step\n";
		setup_timeline_data();
		if (!animation_range.frame_inside_interval(timeline_data.current_frame)) //current frame is outside defined timeline boundary
			return false;

		animation_settings.animation_action =  animation_action_enum::step_forward;
		animation_settings.frame_step       =  timeline_data.frame_step_increment;
		animation_settings.previous_frame   =  timeline_data.current_frame;

		if (!animation_range.frame_inside_interval(timeline_data.current_frame + timeline_data.frame_step_increment)) {//incremented frame is outside defined timeline boundary
			animation_settings.current_frame = animation_range.frame_interval.second;
			timeline_data.current_frame      = animation_range.frame_interval.second;									 // Assign incremented frame to max timeline boundary
		} else {
			animation_settings.current_frame += animation_settings.frame_step;
			timeline_data.current_frame      += timeline_data.frame_step_increment;
		}

		animate_current_frame(true);

		return true;
	}

	bool perform_animation_backward_step() {
//std::cout <<"animation_timeline_panel_class:perform_animation_backward_step 000 : perform_animation_backward_step\n";
		setup_timeline_data();
		if (!animation_range.frame_inside_interval(timeline_data.current_frame))  //current frame is outside defined timeline boundary
			return false;

		animation_settings.animation_action = animation_action_enum::step_back;
		animation_settings.frame_step       = timeline_data.frame_step_increment;
		animation_settings.previous_frame   = timeline_data.current_frame;
		
		if (!animation_range.frame_inside_interval(timeline_data.current_frame - timeline_data.frame_step_increment)) {//incremented frame is outside defined timeline boundary
			animation_settings.current_frame = animation_range.frame_interval.first;
			timeline_data.current_frame      = animation_range.frame_interval.first;									 // Assign incremented frame to max timeline boundary
		} else {
			animation_settings.current_frame -= animation_settings.frame_step;
			timeline_data.current_frame      -= timeline_data.frame_step_increment;
		}

		animate_current_frame(true);

		return true;
	}

	void restore_initial_frame_conditions() {
//std::cout << "animation_timeline_panel_class:restore_initial_frame_conditions 000 : restore_initial_frame_conditions\n";

		stop_animation();
		timeline_data.current_frame      = timeline_data.start_frame;
		animation_settings.current_frame = timeline_data.start_frame;
		animation_settings.animation_action = animation_action_enum::none;
		//animation_settings.current_time  = 0;
		restore_initial_frame();
	}

	void animate_current_frame(bool setup_frame) {
//std::cout << "animation_timeline_panel_class:animate_current_frame 000 : animate_current_frame\n";
		if (animation_range.frame_inside_interval(animation_settings.current_frame)) {// current frame is inside the time range that is to be animated
//std::cout << "animation_timeline_panel_class:animate_current_frame 111 : frame_inside_interval\n";
			 for (timeline_track_group_class timeline_track_group : timeline_tracks_widget.timeline_track_groups) {
//std::cout << "animation_timeline_panel_class:animate_current_frame 222 : frame_inside_interval\n";
				 if (timeline_track_group.group_active()) {
					 for (timeline_track_basis_class *timeline_track : timeline_track_group.timeline_tracks) {
						if (timeline_track->track_active) {
							timeline_int_interval_class* timeline_int_interval = dynamic_cast<timeline_int_interval_class*>(timeline_track);
							if (timeline_int_interval->interval_in_range(timeline_data)) { // Have whole or part of track interval within the timeline frame range
								// In future may have more than one interval in the interval track
								//for (timeline_interval_data_struct<int> timeline_interval : timeline_int_interval->intervals) {
								//	timeline_track_group.animation_object->perform_animation_for_frame(timeline_data.current_frame);
								//}
//std::cout << "animation_timeline_panel_class:animate_current_frame 333 : frame_inside_interval\n";

								//if(timeline_int_interval->frame_in_interval(timeline_data.current_frame)){
								if(timeline_int_interval->frame_in_interval(timeline_data.current_frame)){
									if (setup_frame)
										timeline_track_group.animation_object->setup_animation_data(animation_settings);
									else
										timeline_track_group.animation_object->update_animation_data(animation_settings);
								
									timeline_track_group.animation_object->perform_animation_for_frame(timeline_data.current_frame, timeline_track->track_id);
								}
							}
						}
					 }
				 }
             }
		}
	}

	// ----------------------------------------------

	// this is for time based animation 
	//virtual bool perform_time_step() { return false; }


private:
	bool restore_animation_on_completion = false;

	bool animation_play         = false;
	bool animation_paused       = false;
	bool animation_reverse_play = false;
	bool animation_stopped      = false;
	bool animation_repeat       = false;
	bool animation_record       = false;

	bool animation_play_setup = false;

	void restore_initial_frame() {
		for (animation_object_basis_class animation_object : animation_objects) {
			animation_object.restore_initial_frame();
		}
	}
};
