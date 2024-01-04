#pragma once

#include <Kernels/imgui-docking/imgui.h>

//#include <Universal_FW/im-neo-sequencer/imgui_neo_sequencer.h>
#include <Universal_FW/Timeline/timeline.h>

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>
#include "../Widgets/parameters_widget.h"

class animation_timeline_panel_class : public timeline_class {
public:
    void show() {
        //timeline_data.zoom = 5.0f;// testing only 

        float x_pos = 0.0f, y_pos = 100.0f;

        ImGuiWindow* window = ImGui::GetCurrentWindow();
        timeline_data.wsize = window->Size;
//std::cout << "animation_timeline_panel_class : show : current sequencer : " << timeline_data.wsize.x << ":" << timeline_data.wsize.y << std::endl;

        //ImGui::Begin("Animation-timeline");// Remove/comment out this if do not want dockable panel
       
        display_timeline_controls();

        // Diaplsy frame range zoom slider 
        integer_input("###tlsf", timeline_data.start_frame, x_pos + 10, y_pos, timeline_data.input_box_size.x);
        //ImGui::SliderFloat("scrollbar", &currentFramef, startFrame, endFrame,"%f",0);
        //ImGui::SliderScalar("scrollbar0", ImGuiDataType_U32, &currentFrame, &startFrame, &endFrame, "%d", 0);
        //new_SliderScalar("scrollbar", ImGuiDataType_Float, &currentFramef, &startFramef, &endFramef, "%f", 0);

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
        //bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(window->Size.x - timeline_data.window_x_space + 14, child_height), true);
        ImGui::BeginChild(child_id, ImVec2(window->Size.x - timeline_data.window_x_space + 14, child_height), true);
            draw_timeline_frame_annotation();
        ImGui::EndChild();

        // +++++++++++++++++++++
        //ImGui::SetCursorPosX(x_pos + 75 - 7);
        ImGui::SetCursorPosX(5);



        //timeline_tracks_widget.display_tracks(ImVec2(window->Size.x - timeline_data.window_x_space + 44 + 63+10, window->Size.y - 195), window->Size.x - timeline_data.window_x_space, timeline_data);
        timeline_tracks_widget.display_tracks(ImVec2(window->Size.x - 10, window->Size.y - 195), window->Size.x - timeline_data.window_x_space, timeline_data);
/*
        i = 2;
        child_id = ImGui::GetID((void*)(intptr_t)i);
//std::cout << "animation_timeline_panel_class:: before screen pos : " << ImGui::GetCursorPosY() << std::endl;
        ImGui::BeginChild(child_id, ImVec2(window->Size.x - timeline_data.window_x_space + 44 + 63, window->Size.y - 195), true);
//std::cout << "animation_timeline_panel_class:: before screen pos : " << p0.x << ":" << p0.y << ":" << window->Pos.y << std::endl;

            if (ImGui::CollapsingHeader("Header00", ImGuiTreeNodeFlags_None)) {
                timeline_interval.timeline_int_interval_track(window->Size.x - timeline_data.window_x_space, timeline_data);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
            if (ImGui::CollapsingHeader("Header11", ImGuiTreeNodeFlags_None)) {
                timeline_interval.timeline_int_interval_track(window->Size.x - timeline_data.window_x_space, timeline_data);
            }

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);
            if (ImGui::CollapsingHeader("Header22", ImGuiTreeNodeFlags_None)) {
                timeline_interval.timeline_int_interval_track(window->Size.x - timeline_data.window_x_space, timeline_data);
            }

        ImGui::EndChild();
*/
        // +++++++++++++++++++++

        //ImGui::End;// Remove/comment out this if do not want dockable panel

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
            step_back_animation();

        if (ex_button(ICON_FA_PAUSE "###vauppb", x_pos + 43, y_pos, 40, 23))
            pause_animation();

        if (ex_button(ICON_FA_RETWEET "###vaurpb", x_pos + 86, y_pos, 40, 23))
            repeat_animation();

        if (ex_button(ICON_FA_STEP_FORWARD "###vausfb", x_pos + 129, y_pos, 40, 23))
            step_forward_animation();

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

    bool reverse_animation(){
        animation_reverse_play  = true;
		animation_stopped       = false;
		animation_paused        = false;
		animation_play          = false;
		animation_record        = false;

        return true;
    }

    void stop_animation() {
        animation_stopped       = true;
        timeline_data.current_frame = timeline_data.start_frame;
		//animation_progress      = 0.0f;
		animation_paused        = false;
		animation_play          = false;
		animation_repeat        = false;
		animation_reverse_play  = false;
		animation_record        = false;
    }

    void  record_animation() {
    }

    void play_animation() {
        animation_reverse_play = false;
        animation_stopped = false;
        animation_paused = false;

        if (!animation_play) {
            if (setup_play())
                animation_play = true;
        }
    }

    bool setup_play() {
//printf("setup_play 00");

		animation_stopped       = false;
		animation_paused        = false;
		animation_play          = true;
        timeline_data.current_frame = timeline_data.start_frame;

		return true;
	}

    void step_back_animation() {
        if (timeline_data.current_frame > timeline_data.start_frame)
            timeline_data.current_frame -= 1;
    }

    void pause_animation() {
        animation_paused = !animation_paused;
    }

    void repeat_animation() {
        animation_repeat = !animation_repeat;
        play_animation();
    }

    void step_forward_animation() {
        if (timeline_data.current_frame < timeline_data.end_frame)
            timeline_data.current_frame += 1;
    }

private:
    bool animation_play         = false;
	bool animation_paused       = false;
	bool animation_reverse_play = false;
	bool animation_stopped      = false;
	bool animation_repeat		= false;
	bool animation_record		= false;

    float current_frame_scroll = 0.5f;
};

/*
    if (ImGui::BeginNeoSequencer("Sequencer", &currentFrame, &startFrame, &endFrame, {0, 0},
                                 ImGuiNeoSequencerFlags_EnableSelection |
                                 ImGuiNeoSequencerFlags_Selection_EnableDragging |
                                 ImGuiNeoSequencerFlags_Selection_EnableDeletion))
    {
        if (ImGui::BeginNeoGroup("Transform", &transformOpen))
        {

            if (ImGui::BeginNeoTimelineEx("Position"))
            {
                for (auto&& v: keys)
                {
                    ImGui::NeoKeyframe(&v);
                    // Per keyframe code here
                }


                if (doDelete)
                {
                    uint32_t count = ImGui::GetNeoKeyframeSelectionSize();

                    ImGui::FrameIndexType * toRemove = new ImGui::FrameIndexType[count];

                    ImGui::GetNeoKeyframeSelection(toRemove);

                    //Delete keyframes from your structure
                }
                ImGui::EndNeoTimeLine();
            }
            ImGui::EndNeoGroup();
        }

        ImGui::EndNeoSequencer();
    }


*/