#pragma once

//
// Created by Matty on 2022-01-28.
//

#include "imgui.h"
#include "imgui_internal.h"
#include <cstdint>

struct Vec2Pair {
    ImVec2 a;
    ImVec2 b;
};

class neo_sequencer_internal_class {
public:
    void  RenderNeoSequencerBackground(const ImVec4& color, const ImVec2& cursor, const ImVec2& size, ImDrawList* drawList = nullptr, float sequencerRounding = 0.0f);
    void  RenderNeoSequencerTopBarBackground(const ImVec4& color, const ImVec2& cursor, const ImVec2& size, ImDrawList* drawList = nullptr, float sequencerRounding = 0.0f);
    
    // Draw timeline tickmarks and annotation
    void  RenderNeoSequencerTopBarOverlay(float zoom, float valuesWidth, uint32_t startFrame, uint32_t endFrame, uint32_t offsetFrame, const ImVec2& cursor, const ImVec2& size, ImDrawList* drawList = nullptr, bool drawFrameLines = true, bool drawFrameText = true, float maxPixelsPerTick = -1.0f);
    void  RenderNeoTimelineLabel(const char* label, const ImVec2& cursor, const ImVec2& size, const ImVec4& color, bool isGroup = false, bool isOpen = false, ImDrawList* drawList = nullptr);
    void  RenderNeoTimelane(bool selected, const ImVec2& cursor, const ImVec2& size, const ImVec4& highlightColor, ImDrawList* drawList = nullptr);
    void  RenderNeoTimelinesBorder(const ImVec4& color, const ImVec2& cursor, const ImVec2& size, ImDrawList* drawList = nullptr, float rounding = 0.0f, float borderSize = 1.0f);
    
    // Draw sequencer current frame cursor widget 
    void  RenderNeoSequencerCurrentFrame(const ImVec4& color, const ImVec4& topColor, const ImRect& pointerBB, float timelineHeight, float lineWidth = 1.0f, ImDrawList* drawList = nullptr);

    Vec2Pair neo_sequencer_internal_class::getCurrentFrameLine(const ImRect& pointerBB, float timelineHeight);

    // Calculate number pixel spacing on screen per frame 
    float GetPerFrameWidth(float totalSizeX, float valuesWidth, uint32_t endFrame, uint32_t startFrame, float zoom);

    // ++++++++++++++++++
    virtual void display_timeline_controls() {}
};
