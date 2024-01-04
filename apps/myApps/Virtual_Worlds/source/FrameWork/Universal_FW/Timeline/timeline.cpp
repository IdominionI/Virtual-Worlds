#define IMGUI_DEFINE_MATH_OPERATORS // This must be placed here for native Imgui math operators to function

#include "timeline.h"

#include <iostream> // For cout only: delete/comment out when finished

#include <FrameWork/Kernels/ImGuiEx/imgui_widgets.h>

// timeline_zoom_widget is a modified copy of the ImGui SliderScalar widget for integer scrolling.

// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a slider, they are all required.
// Read code of e.g. SliderFloat(), SliderInt() etc. or examples in 'Demo->Widgets->Data Types' to understand how to use this function directly.
bool timeline_class::timeline_integer_zoom_widget(const char* label, ImGuiDataType data_type, float cursor_x, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    //const float w = ImGui::CalcItemWidth();
    const float w = window->Size.x - timeline_data.window_x_space; // width of slider widget rectangle

    const ImVec2 label_size = { 0.0,15.0 };// *****
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ImGui::ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
    const bool make_active = (clicked || g.NavActivateId == id);

    if (make_active)
    {
        ImGui::SetActiveID(id, window);
        ImGui::SetFocusID(id, window);
        ImGui::FocusWindow(window);
        g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
    }

    // Draw frame
    const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
    ImGui::RenderNavHighlight(frame_bb, id);
    ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);

    // Slider behavior
    ImRect grab_bb;

    if (timeline_data.zoom < 1.0f) timeline_data.zoom = 1.0f;

    int slider_min = *(int*)p_min;
    int slider_max = *(int*)p_max;
    int slider_value = *(int*)p_data;

    int interval = slider_max - slider_min;

    int slider_width = (int)((float)slider_max - (float)slider_min) / timeline_data.zoom;

    int new_slider_max = (int)ceil((float)slider_max - ((float)slider_max - (float)slider_min) / timeline_data.zoom);

    bool value_changed = false;

    if (slider_value > new_slider_max)
        *(int*)p_data = new_slider_max;

    if (timeline_data.zoom > 1.0) {
        timeline_data.zoom_start_frame = *(int*)p_data;
        timeline_data.zoom_end_frame = timeline_data.zoom_start_frame + slider_width;
    } else{
        timeline_data.zoom_start_frame = timeline_data.start_frame;
        timeline_data.zoom_end_frame   = timeline_data.end_frame;
    }


    value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);

//std::cout << "animation_timeline_panel_class::new_SliderScalar : 1111 " << grab_bb.Min.x << ":" << grab_bb.Max.x << ":" << w << ":" << window->DC.CursorPos.x << std::endl;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : 1111 " << timeline_data.zoom_start_frame << ":" << timeline_data.zoom_end_frame << ":" << w << ":" << window->DC.CursorPos.x << std::endl;

    // bb_width is the magic that needs to be added to the slider width to allow the slider button width to be correct.
    // Seems some ImGui internal calculations and assignments or slider button width are created in SliderBehavior that
    // then are used in AddRectFilled that create a padding space in the widget.
    // cursor_x also critical if display of slider is to be consistent so that if SliderBehavior performs any changes
    // when it interprets the user to try and slide into the last zoom zone, the button is of the correct size.
    // grab_bb is in screen coordinates which wasted a lot of time figuring out !!!!
    float bb_width = grab_bb.Max.x - grab_bb.Min.x;

    float zoom_width = w / timeline_data.zoom;
    float x_cursor_pos = window->Pos.x;

    if (slider_value >= new_slider_max) {
        slider_value = new_slider_max;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : 0000 " << slider_min << ":" << slider_max << ":" << slider_width << ":" << std::endl;          
        if (interval % int(timeline_data.zoom) == 0)
            if (timeline_data.zoom == 1.0)
                grab_bb.Min.x = x_cursor_pos + w - zoom_width + cursor_x + 2.0; // Zoom = 1.0 
            else
                grab_bb.Min.x = x_cursor_pos + w - zoom_width - bb_width / timeline_data.zoom + cursor_x - 2.0f; // zoom = X
        //grab_bb.Min.x = x_cursor_pos + w - zoom_width  + cursor_x + 2.0 ; // Zoom = 1.0 
        //grab_bb.Min.x = x_cursor_pos + w - zoom_width - bb_width/2.0f + cursor_x - 2.0f; // zoom =2.0
       // grab_bb.Min.x = x_cursor_pos + w - zoom_width - bb_width / 3.0f + cursor_x  ; // Zoom = 3
        else
            grab_bb.Min.x = x_cursor_pos + w - zoom_width + cursor_x - 2.0f;
    }

    if (value_changed)
        ImGui::MarkItemEdited(id);

    if (interval % int(timeline_data.zoom) == 0)
        if (timeline_data.zoom == 1.0f)
            grab_bb.Max.x = grab_bb.Min.x + w / timeline_data.zoom - 4.0f;// Zoom = 1.0
        else
            grab_bb.Max.x = grab_bb.Min.x + w / timeline_data.zoom + bb_width / timeline_data.zoom - 2.0f;// zoom = X

    //grab_bb.Max.x = grab_bb.Min.x + w/zoom - 4.0f;// Zoom = 1.0
    //grab_bb.Max.x = grab_bb.Min.x + w/zoom + bb_width/2.0f - 2.0f;// zoom =2.0
    //grab_bb.Max.x = grab_bb.Min.x + w / zoom + bb_width / 3.0f - 2.0f; //+ cursor_x;// Zoom = 
    else
        grab_bb.Max.x = grab_bb.Min.x + w / timeline_data.zoom - 2.0f;// zoom = 1.0 : 2.0 : 3.0

//std::cout << "animation_timeline_panel_class::new_SliderScalar : 2222 " << grab_bb.Min.x << ":" << grab_bb.Max.x << ":" << window->Pos.x << ":" << std::endl;
//std::cout << "animation_timeline_panel_class::new_SliderScalar : 3333 " << frame_bb.Min.x << ":" << frame_bb.Max.x << ":" << *(int*)p_data << ":" << std::endl;

    // Render grab
    if (grab_bb.Max.x > grab_bb.Min.x) {
        window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);
    }

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    char value_buf[64];
    const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
    if (g.LogEnabled)
        ImGui::LogSetNextTextDecoration("{", "}");

    ImGui::RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}


void timeline_class::draw_timeline_frame_annotation() {
    ImDrawList *drawList           = ImGui::GetWindowDrawList();
    timeline_data.annotation_wsize = ImGui::GetWindowSize(); // Gets the size of the widget

    ImVec2 p0 = ImGui::GetCursorScreenPos();

    int no_frames = timeline_data.zoom_end_frame - timeline_data.zoom_start_frame;

    int timeline_annotation_width = timeline_data.annotation_wsize.x-14; // Seems the widget display size has some padding around it.

    float frames_per_pixel = float(no_frames) / float(timeline_annotation_width);
    float pixels_per_frame = 1.0f / frames_per_pixel;

    int min_major_tick_interval_pixel_width = 25;
    int max_major_tick_resolution           = 6;
    int min_minor_tick_interval_pixel_width = 5;

    int no_major_ticks = timeline_annotation_width / min_major_tick_interval_pixel_width;

    if (no_major_ticks < 1) return; // cannot display timeline annotation, widget width too small

    // Find major tick resolution
    int major_tick_resolution = 0;
    int major_tick_factor = 1;

    while (no_frames/major_tick_factor > 10) {
       major_tick_resolution++;
        major_tick_factor *= 10;
    }
//std::cout << "timeline_class::draw_timeline_frame_annotation 000: " << major_tick_resolution << ":" << major_tick_factor << std::endl;

    if (major_tick_resolution > max_major_tick_resolution) return; // major tick annotation too large to display

    int minor_tick_resolution = major_tick_resolution - 1;
    no_major_ticks        = no_frames / major_tick_factor;
//std::cout << "timeline_class::draw_timeline_frame_annotation 111: " << no_major_ticks << ":" << timeline_annotation_width << std::endl;

    if (no_major_ticks > timeline_annotation_width / min_major_tick_interval_pixel_width) return; // not enough screnn space to display annotation.

    // find first major tick frame
    int major_tick_frame_offset;
    if(timeline_data.zoom_start_frame % major_tick_factor == 0) 
        major_tick_frame_offset = 0;
    else
        major_tick_frame_offset = major_tick_factor - timeline_data.zoom_start_frame % major_tick_factor;

    int   initial_major_tick_frame  = timeline_data.zoom_start_frame + major_tick_frame_offset;
    float initial_major_tick_offset = p0.x + major_tick_frame_offset * pixels_per_frame;
    float major_interval_width      = major_tick_factor * pixels_per_frame;
    float minor_interval_width = 0;
    if(major_tick_factor > 1)
        minor_interval_width = (major_tick_factor/10.0f) * pixels_per_frame;

//std::cout << "timeline_class::draw_timeline_frame_annotation 222A: " << frame_offset << ":" << timeline_data.zoom_start_frame % major_tick_factor << ":" << pixels_per_frame << std::endl;
//std::cout << "timeline_class::draw_timeline_frame_annotation 222: " << initial_major_tick_frame << ":" << (timeline_data.zoom_start_frame % major_tick_factor) * pixels_per_frame << ":" << interval_width << ":" << pixels_per_frame << std::endl;

    for (int i = 0; i < no_major_ticks+1; i++) {
        // Major ticks marks
        ImVec2 major_p1 = { initial_major_tick_offset + major_interval_width *i,p0.y + timeline_data.annotation_wsize.y*0.3f };
        ImVec2 major_p2 = { initial_major_tick_offset + major_interval_width *i,p0.y - 3.0f };

        drawList->AddLine(major_p1, major_p2, IM_COL32_WHITE, 1.0f);

        if (major_tick_factor > 1) {
            // Minor tick marks before first major tick mark
            for (int j = 0; j < major_tick_frame_offset; j++) {
                int minor_tick_length = timeline_data.annotation_wsize.y*0.15f;
                if (major_tick_frame_offset - j == 5)
                    minor_tick_length = timeline_data.annotation_wsize.y*.25f;
                ImVec2 minor_p1 = { p0.x + minor_interval_width * j,p0.y + minor_tick_length };
                ImVec2 minor_p2 = { p0.x + minor_interval_width * j,p0.y - 3.0f };

                drawList->AddLine(minor_p1, minor_p2, IM_COL32(255, 0, 0, 255), 1.0f);
            }

            // Minor tick marks between major tick marks
            for (int j = 1; j < 10; j++) {
                int minor_tick_length = timeline_data.annotation_wsize.y*0.15f;
                if (j == 5)
                    minor_tick_length = timeline_data.annotation_wsize.y*0.25f;

                ImVec2 minor_p1 = { initial_major_tick_offset + major_interval_width * i + minor_interval_width * j,p0.y + minor_tick_length };
                ImVec2 minor_p2 = { initial_major_tick_offset + major_interval_width * i + minor_interval_width * j,p0.y - 3.0f };

                drawList->AddLine(minor_p1, minor_p2, IM_COL32(255, 0, 0, 255), 1.0f);
            }
        }

        std::string text = std::to_string(initial_major_tick_frame + i* major_tick_factor);
        if (text.size() > 0) {
            drawList->AddText(NULL, 0, { major_p1.x + 2.0f, p0.y + timeline_data.annotation_wsize.y*0.25f }, IM_COL32_WHITE, text.c_str());
        }
    }

    // Draw line representing current frame 
    if (timeline_data.inside_timeline_display_range(timeline_data.current_frame)) {
        int current_frame_offset = timeline_data.current_frame-timeline_data.zoom_start_frame;

        float pixel_offset = current_frame_offset * pixels_per_frame;

        ImVec2 cf_p1 = { p0.x + pixel_offset,p0.y + timeline_data.annotation_wsize.y };
        ImVec2 cf_p2 = { p0.x + pixel_offset,p0.y - 3.0f };

        drawList->AddLine(cf_p1, cf_p2, IM_COL32(0,255, 0, 255), 1.0f);
    }

}


// Note: p_data, p_min and p_max are _pointers_ to a memory address holding the data. For a slider, they are all required.
// Read code of e.g. SliderFloat(), SliderInt() etc. or examples in 'Demo->Widgets->Data Types' to understand how to use this function directly.
bool timeline_class::timeline_integer_slider_widget(const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags){
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const float w = window->Size.x - timeline_data.window_x_space; // width of slider widget rectangle

//std::cout << "animation_timeline_panel_class::timeline_interval_track_widget : %%%%% " << window->Size.x << ":" << w << ":" << timeline_data.window_x_space << std::endl;

    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
    const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

    const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
    ImGui::ItemSize(total_bb, style.FramePadding.y);
    if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
        return false;

    // Default format string when passing NULL
    if (format == NULL)
        format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

    const bool hovered = ImGui::ItemHoverable(frame_bb, id, g.LastItemData.InFlags);
    bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
    if (!temp_input_is_active)
    {
        // Tabbing or CTRL-clicking on Slider turns it into an input box
        const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
        const bool clicked = hovered && ImGui::IsMouseClicked(0, id);
        const bool make_active = (input_requested_by_tabbing || clicked || g.NavActivateId == id);
        if (make_active && clicked)
            ImGui::SetKeyOwner(ImGuiKey_MouseLeft, id);
        if (make_active && temp_input_allowed)
            if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || (g.NavActivateId == id && (g.NavActivateFlags & ImGuiActivateFlags_PreferInput)))
                temp_input_is_active = true;

        if (make_active && !temp_input_is_active)
        {
            ImGui::SetActiveID(id, window);
            ImGui::SetFocusID(id, window);
            ImGui::FocusWindow(window);
            g.ActiveIdUsingNavDirMask |= (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
        }
    }

    if (temp_input_is_active)
    {
        // Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
        const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0;
        return ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL, is_clamp_input ? p_max : NULL);
    }

    // Draw frame
    if (display_int_slider_frame) {
        const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
        ImGui::RenderNavHighlight(frame_bb, id);
        ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, g.Style.FrameRounding);
    }

    // Slider behavior
    ImRect grab_bb;
    const bool value_changed = ImGui::SliderBehavior(frame_bb, id, data_type, p_data, p_min, p_max, format, flags, &grab_bb);
    
    if (value_changed) {
        ImGui::MarkItemEdited(id);

        int   no_frames = timeline_data.zoom_end_frame - timeline_data.zoom_start_frame;
        float frame_percentage = 1.0f / float(no_frames);

        float slider_value = *(float*)p_data;

//std::cout << "animation_timeline_panel_class::new_SliderScalar : 1111 " << no_frames << ":" << frame_percentage << ":" << slider_value << ":" << std::endl;

        timeline_data.current_frame = timeline_data.zoom_start_frame + int(slider_value/frame_percentage);
    }


    // Render grab
    // Only render a grab widget rectangle if the current frame lies within the current zoom range
    if (timeline_data.inside_timeline_display_range(timeline_data.current_frame)) {
        float x_cursor_pos = window->Pos.x;

        int   no_frames        = timeline_data.zoom_end_frame - timeline_data.zoom_start_frame;
        float frame_percentage = float(timeline_data.current_frame- timeline_data.zoom_start_frame) / float(no_frames);

        float grab_bb_width = grab_bb.Max.x - grab_bb.Min.x;

        grab_bb.Min.x = x_cursor_pos + 75 + w * frame_percentage - grab_bb_width/2.0f;
        grab_bb.Max.x = grab_bb.Min.x + grab_bb_width;

//std::cout << "animation_timeline_panel_class::timeline_integer_slider_widget : AAAA " << x_cursor_pos << ":" << grab_bb.Min.x << ":" << w << ":" << std::endl;
        if (grab_bb.Max.x > grab_bb.Min.x)
            window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);
    }

    // Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
    if (display_int_slider_value) {
        char value_buf[64];
        const char* value_buf_end = value_buf + ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format);
        if (g.LogEnabled)
            ImGui::LogSetNextTextDecoration("{", "}");
        ImGui::RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

        if (label_size.x > 0.0f)
            ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
    }

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | (temp_input_allowed ? ImGuiItemStatusFlags_Inputable : 0));
    return value_changed;
}
