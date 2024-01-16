#include "timeline_interval.h"

#include "timeline.h"

#include <iostream>// Testing only for cout use: delete comment out when not used

bool timeline_int_interval_class::timeline_int_interval_track(float widget_width, int_timeline_parameter_data_struct_type timeline_data ) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->ParentWindow == nullptr) return false;
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

//std::cout << "animation_timeline_panel_class::timeline_interval_track_widget : 00000 " << window->Size.x << ":" << widget_width << ":" << timeline_data.window_x_space << std::endl;

    ImGui::ItemSize({ widget_width,2.0 });// Offers some padding at the top : delete if found to cause problems or not needed

    sc_pos = ImGui::GetCursorScreenPos();

    //Draw label
    ImGui::Text(label.c_str());
    ImGui::SetCursorScreenPos(sc_pos);

    // Draw frame
    ImGui::RenderFrame({ sc_pos.x+68 ,sc_pos.y - 10.0f }, { sc_pos.x + widget_width + 68.0f, sc_pos.y + 23.0f }, IM_COL32(25, 25, 25, 255), true, g.Style.FrameRounding);

   
    no_frames = timeline_data.number_displayed_frames();

    frames_per_pixel = float(no_frames) / widget_width;
    pixels_per_frame = 1.0f / frames_per_pixel;

    // for future multiple intervals per track capability
    // modify current perform_interval_behavior and have within it
    // float min_button_pos = interval.interval_min_frame;
    // float max_button_pos = interval.interval_max_frame;
    // and other assignments as neccessary
    //for (timeline_interval_data_struct interval: intervals) {
    //    bool value_changed = perform_interval_behavior(window, widget_width, timeline_data, interval);
    //}

    bool value_changed = perform_interval_behavior(window, widget_width, timeline_data);

    // +++++++++++++++++
    if (value_changed) {
        if (animation_object && animation_object->intervals.size() > 0) {
            animation_object->intervals[0].frame_interval.first  = timeline_interval_min_frame;
            animation_object->intervals[0].frame_interval.second = timeline_interval_max_frame;
        }
    }
    // +++++++++++++++++

 //   IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}

// This function is only valid for the case of a single action interval for one track
// Goal is to have the ability to have multiple intervals per track which brings in multiple
// issues of preventing overlap and editing of splitting/merging and adding new intervals via
// menu or hotkeys etc. Future feature that is not required at this time.
bool timeline_int_interval_class::perform_interval_behavior(ImGuiWindow* window,
								                            float widget_width,
								                            int_timeline_parameter_data_struct_type timeline_data){
    bool value_changed = false;

    float min_button_pos = 0.0f;
    float max_button_pos = 0.0f;
//std::cout << "animation_timeline_panel_class:: after screen pos : " << sc_pos.x << ":" << sc_pos.y << ":" << window->Pos.y << std::endl;

    // Perform interval minimum widget behaviour
    if (timeline_data.inside_timeline_range(timeline_interval_min_frame)) {
        float min_frame_percentage = float(timeline_interval_min_frame - timeline_data.zoom_start_frame) / float(no_frames);
        min_button_pos = min_frame_percentage * widget_width + 68;

        ImGui::SetCursorPosX(min_button_pos - 7.0f);
        //ImGui::SetCursorPosY(16.0f); // Use this and the whole application widgets become unresponsive and cannot be selected !!!!!!

        std::string button_id = "<:###iminb0" + std::to_string(button_id_add);
        ImGui::Button(button_id.c_str(), { 14,14 });

        if (ImGui::IsItemActive()) {
//std::cout << "animation_timeline_panel_class::new_SliderScalar :min button active " << std::endl;
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                if (!min_button_initial) {
                    min_button_initial = true;
                    prev_min_button_pos = min_button_pos;
                    current_min_button_pos = min_button_pos;
                    timeline_interval_prev_min_frame = timeline_interval_min_frame;
                }

                ImVec2 delta = ImGui::GetMouseDragDelta(0, 1.0f);

                current_min_button_pos = prev_min_button_pos + delta.x;

                current_min_frame = floor(float(timeline_interval_prev_min_frame) + delta.x * frames_per_pixel);
//std::cout << "animation_timeline_panel_class::new_SliderScalar : Min 000 " << delta.x << ":" << delta.y << ":" << current_min_frame << ":" << std::endl;
                if (current_min_frame < timeline_data.zoom_start_frame) {
                    current_min_frame = timeline_data.zoom_start_frame;
                }
                else {
                    if (current_min_frame > timeline_data.zoom_end_frame && current_min_frame < timeline_interval_max_frame) {
                        current_min_frame = timeline_data.zoom_end_frame;
                    }
                    else {
                        if (current_min_frame > timeline_interval_max_frame) {
                            current_min_frame = timeline_interval_max_frame;
                        }
                    }
                }

                timeline_interval_min_frame = current_min_frame;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : Min 111 " << current_min_frame << ":" << current_min_button_pos << ":" << ":" << std::endl;
            }
            else {
                min_button_initial = false;
            }
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && min_button_initial) {
            min_button_initial = false;
            timeline_interval_min_frame = current_min_frame;
            if (timeline_interval_prev_min_frame != timeline_interval_min_frame) value_changed = true;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : min mouse released outside drag  " << current_max_frame << ":" << ":" << ":" << std::endl;
        }
    }

    if(!timeline_data.inside_timeline_display_range(timeline_interval_min_frame)) ImGui::SameLine();// Need this to have buttons on the same Y cursor location
    // Perform interval maximum widget behaviour
    if (timeline_data.inside_timeline_range(timeline_interval_max_frame)) {
        if(timeline_data.inside_timeline_display_range(timeline_interval_min_frame)) ImGui::SameLine();// Need this to have buttons on the same Y cursor location
        float max_frame_percentage = float(timeline_interval_max_frame - timeline_data.zoom_start_frame) / float(no_frames);
        max_button_pos = max_frame_percentage * widget_width + 68;

        ImGui::SetCursorPosX(max_button_pos+7.0f);
        //ImGui::SetCursorPosY(16.0f); // Use this and the whole application widgets become unresponsive and cannot be selected !!!!!!
        std::string button_id = ">:###imaxb0" + std::to_string(button_id_add);
        ImGui::Button(button_id.c_str(), {14,14});
        //ImGui::Button(">:###imaxb00", { 14,14 });

        // The stupid thing about this if statement is that this only performs any task assigned to slecting this button after 
        // the mouse button is released and also does not allow any other interactive code to be executed withing the if satement.
        // Crazy !!!!!
//        if (ImGui::Button(">:###imaxb00", { 14,14 })) { 
//std::cout << "animation_timeline_panel_class::new_SliderScalar :button selected " << std::endl;
        //}

        if (ImGui::IsItemActive()) {
//std::cout << "animation_timeline_panel_class::new_SliderScalar :max button active " << std::endl;
        
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
                if (!max_button_initial) {
                    max_button_initial = true;
                    prev_max_button_pos = max_button_pos;
                    current_max_button_pos = max_button_pos;
                    timeline_interval_prev_max_frame = timeline_interval_max_frame;
                }

                ImVec2 delta = ImGui::GetMouseDragDelta(0, 1.0f);

                current_max_button_pos = prev_max_button_pos + delta.x;
                
                current_max_frame = floor(float(timeline_interval_prev_max_frame) + delta.x * frames_per_pixel);
//std::cout << "animation_timeline_panel_class::new_SliderScalar : 000 " << delta.x << ":" << delta.y << ":" << current_max_frame << ":" << std::endl;
                if (current_max_frame > timeline_data.zoom_end_frame) {
                    current_max_frame = timeline_data.zoom_end_frame;
                } else {
                    if (current_max_frame < timeline_data.zoom_start_frame && current_max_frame > timeline_interval_min_frame) {
                        current_max_frame = timeline_data.zoom_start_frame;
                    } else {
                        if (current_max_frame < timeline_interval_min_frame) {
                            current_max_frame = timeline_interval_min_frame;
                        }
                    }
                }
                timeline_interval_max_frame = current_max_frame;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : 111 " << current_max_frame << ":" << current_max_button_pos << ":" << ":" << std::endl;
            }else{
               max_button_initial = false;
            }
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left) && max_button_initial) {
            max_button_initial = false;
            timeline_interval_max_frame = current_max_frame;
            if (timeline_interval_prev_max_frame != timeline_interval_max_frame) value_changed = true;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : max mouse released outside drag  " << current_max_frame << ":" << ":" << ":" << std::endl;
        }
    }

    draw_timeline_interval(window, widget_width, timeline_data, min_button_pos, max_button_pos);

    return value_changed;
}


void timeline_int_interval_class::draw_timeline_interval(ImGuiWindow* window, 
                                                         float widget_width,
                                                         int_timeline_parameter_data_struct_type timeline_data,
                                                         float min_button_pos,float max_button_pos) {
    float min_interval_frame_draw = -1.0f;
    float max_interval_frame_draw = -1.0f;

    if (interval_in_display_range(timeline_data))
    {
        if (timeline_data.inside_timeline_display_range(timeline_interval_min_frame))
            min_interval_frame_draw = min_button_pos;
        else
            if (timeline_data.below_timeline_display_range(timeline_interval_min_frame))
                min_interval_frame_draw = 0.0 + 68.0f;

        if (timeline_data.inside_timeline_display_range(timeline_interval_max_frame))
            max_interval_frame_draw = max_button_pos;
        else
            if (timeline_data.above_timeline_display_range(timeline_interval_max_frame))
                max_interval_frame_draw = widget_width + 68.0f;

        if (min_interval_frame_draw >= 0.0f || max_interval_frame_draw >= 0.0f) {
            window->DrawList->AddRect({ sc_pos.x + min_interval_frame_draw,sc_pos.y + 20.0f }, { sc_pos.x + max_interval_frame_draw,sc_pos.y - 6 }, IM_COL32(255, 255, 0, 255), 1.0f);
        }


        // Draw line representing current frame
        int current_frame_offset = timeline_data.current_frame - timeline_data.zoom_start_frame;

        float pixel_offset = current_frame_offset * pixels_per_frame + 68;

        ImVec2 cf_p1 = { sc_pos.x + pixel_offset,sc_pos.y + 20.f };
        ImVec2 cf_p2 = { sc_pos.x + pixel_offset,sc_pos.y - 6.0f };

        window->DrawList->AddLine(cf_p1, cf_p2, IM_COL32(0, 255, 0, 255), 1.0f);

    }
    else
        ImGui::SetCursorScreenPos({ sc_pos.x,sc_pos.y + 20.0f }); // Need this to ensure any following ImGui widgets do not over lay on this one and are aligned properly : Critiacal !!!
}