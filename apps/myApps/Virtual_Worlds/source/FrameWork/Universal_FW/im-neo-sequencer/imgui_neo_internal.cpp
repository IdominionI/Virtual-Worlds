//
// Created by Matty on 2022-01-28.
//

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_neo_internal.h"
#include "imgui_internal.h"
#include <cstdint>

#include <string>

#include <iostream> // Testing only : delete/comment out when finised

void neo_sequencer_internal_class::RenderNeoSequencerBackground(const ImVec4 &color, const ImVec2 & cursor, const ImVec2 &size, ImDrawList * drawList, float sequencerRounding) {
    if(!drawList) drawList = ImGui::GetWindowDrawList();

    ImRect area = {cursor, cursor + size};

    drawList->AddRectFilled(area.Min, area.Max, ImGui::ColorConvertFloat4ToU32(color), sequencerRounding);
}

void neo_sequencer_internal_class::RenderNeoSequencerTopBarBackground(const ImVec4 &color, const ImVec2 &cursor, const ImVec2 &size,
                                        ImDrawList *drawList, float sequencerRounding) {
    if(!drawList) drawList = ImGui::GetWindowDrawList();

    ImRect barArea = {cursor, cursor + size};

    drawList->AddRectFilled(barArea.Min, barArea.Max, ImGui::ColorConvertFloat4ToU32(color), sequencerRounding);
}

// Draw timeline tickmarks and annotation
void neo_sequencer_internal_class::RenderNeoSequencerTopBarOverlay(float zoom, float valuesWidth,uint32_t startFrame, uint32_t endFrame, uint32_t offsetFrame, const ImVec2 &cursor, const ImVec2 &size,
                                                                    ImDrawList *drawList, bool drawFrameLines,
                                                                    bool drawFrameText, float maxPixelsPerTick) {
    if(!drawList) drawList = ImGui::GetWindowDrawList();

    ImGuiStyle& style = ImGui::GetStyle();

    ImRect barArea = {cursor + ImVec2{style.FramePadding.x + valuesWidth,style.FramePadding.y}, cursor + size };

    uint32_t viewEnd   = endFrame + offsetFrame;
    uint32_t viewStart = startFrame + offsetFrame;

    if(drawFrameLines) {
        int32_t count = (int32_t)((float)((viewEnd + 1) - viewStart) / zoom);

        uint32_t  counter = 0;

        uint32_t primaryFrames = pow(10, counter++);
        uint32_t secondaryFrames = pow(10, counter);

        float perFrameWidth = GetPerFrameWidth(size.x, valuesWidth, endFrame, startFrame, zoom);

        if(perFrameWidth <= 0.0f) return;

        // This while statement is at the root of the problem of this
        // function creating annotation probelms if the number of frames
        // > 1000 and especially between 1000-10000 frame interval.
        // Tried all day to fix this but need a completely new approach
        // that would need to rewrite code elsewhare to fix such an annoying minor bug.
        while (perFrameWidth < maxPixelsPerTick)
        {
            primaryFrames   = pow(10, counter);
            counter++;
            secondaryFrames = pow(10, counter);

            perFrameWidth *= (float)primaryFrames;
        }

        if(primaryFrames == 0 || secondaryFrames == 0) {
            primaryFrames = 1;
            secondaryFrames = 10;
        }

        //****
        // This is added so as to have the correct labeling of major ticks
        // and not have over flow of labeling and tick drawing when have
        // interval > 1000. Seems this algorthim can only handle intervals 
        // <= 1000 frames. Could not find a method to correct this as problem
        // origintes in in the while (perFrameWidth < maxPixelsPerTick) above
        if (counter > 2) {
            primaryFrames   *= 10;
            secondaryFrames *= 10;
        }
        //****

        // i - frame number : count - number frames 
        for(int32_t i = 0; i < count; i++) {

            bool primaryFrame   = ((viewStart + i) % primaryFrames == 0);
            bool secondaryFrame = ((viewStart + i) % secondaryFrames == 0);

            if (!primaryFrame && !secondaryFrame) continue;

            float lineHeight = secondaryFrame ? barArea.GetSize().y : barArea.GetSize().y / 2.0f;

            ImVec2 p1 = {barArea.Min.x + (float)i * (perFrameWidth / (float)primaryFrames), barArea.Max.y};
            ImVec2 p2 = {barArea.Min.x + (float)i * (perFrameWidth / (float)primaryFrames), barArea.Max.y - lineHeight};

            drawList->AddLine(p1,p2, IM_COL32_WHITE, 1.0f);

            if(drawFrameText && secondaryFrame) {
                //char text[10];
                //int printRes = snprintf(text, sizeof(text), "%i", viewStart + i);
                //if (printRes > 0) {
                //    drawList->AddText(NULL, 0, { p1.x + 2.0f, barArea.Min.y }, IM_COL32_WHITE, text);
                //}
 
                //**** this is easier
                std::string text = std::to_string(viewStart + i);
                if (text.size() > 0) {
                    drawList->AddText(NULL, 0, { p1.x + 2.0f, barArea.Min.y }, IM_COL32_WHITE, text.c_str());
                }
                //****
            }
        }

    }

}

void neo_sequencer_internal_class::RenderNeoTimelineLabel(const char * label,const ImVec2 & cursor,const ImVec2 & size, const ImVec4& color,bool isGroup, bool isOpen, ImDrawList *drawList)
{
    const auto& imStyle = ImGui::GetStyle();

    if(!drawList) drawList = ImGui::GetWindowDrawList();

    auto c = cursor;

    if(isGroup) {
        ImGui::RenderArrow(drawList,c,IM_COL32_WHITE,isOpen ? ImGuiDir_Down : ImGuiDir_Right);
        c.x += size.y + imStyle.ItemSpacing.x;
    }

    drawList->AddText(c, ImGui::ColorConvertFloat4ToU32(color),label, ImGui::FindRenderedTextEnd(label));
}

void neo_sequencer_internal_class::RenderNeoTimelinesBorder(const ImVec4 &color,const ImVec2 &cursor,const ImVec2 &size,ImDrawList *drawList,float rounding,float borderSize)
{
    if(!drawList) drawList = ImGui::GetWindowDrawList();

    drawList->AddRect(cursor,cursor + size, ImGui::ColorConvertFloat4ToU32(color),rounding, 0, borderSize);
}

void neo_sequencer_internal_class::RenderNeoTimelane(bool selected,const ImVec2 & cursor, const ImVec2& size, const ImVec4& highlightColor, ImDrawList *drawList) {
    if(!drawList) drawList = ImGui::GetWindowDrawList();

    if(selected) {
        const ImRect area = {cursor, cursor + size};
        drawList->AddRectFilled(area.Min, area.Max, ImGui::ColorConvertFloat4ToU32(highlightColor));
    }
}

// Calculate number pixel spacing on screen per frame 
float neo_sequencer_internal_class::GetPerFrameWidth(float totalSizeX, float valuesWidth, uint32_t endFrame, uint32_t startFrame, float zoom) {
    ImGuiStyle& imStyle = ImGui::GetStyle();

    float size = totalSizeX - valuesWidth - imStyle.FramePadding.x;

    int count = (endFrame + 1) - startFrame;

//std::cout << "neo_sequencer_internal_class::GetPerFrameWidth : " << totalSizeX << ":" << valuesWidth << ":" << imStyle.FramePadding.x << ":" << size << ":" << count << std::endl;


    return ((size / (float)count) * zoom);
}

Vec2Pair neo_sequencer_internal_class::getCurrentFrameLine(const ImRect & pointerBB, float timelineHeight) {
    const auto center = ImVec2{pointerBB.Min.x, pointerBB.Max.y} + ImVec2{pointerBB.GetSize().x / 2.0f, 0};

    return Vec2Pair{ center, center + ImVec2{0, timelineHeight} };
}


// Draw sequencer current frame cursor widget 
void neo_sequencer_internal_class::RenderNeoSequencerCurrentFrame(const ImVec4 &color, const ImVec4 &topColor, const ImRect &pointerBB,
                                            float timelineHeight, float lineWidth, ImDrawList *drawList) {
    if(!drawList) drawList = ImGui::GetWindowDrawList();

    const auto pair = getCurrentFrameLine(pointerBB, timelineHeight);

    drawList->AddLine(pair.a, pair.b, ImGui::ColorConvertFloat4ToU32(color), lineWidth);

    drawList->PopClipRect();

    { //Top pointer has custom shape, we have to create it
        const auto size = pointerBB.GetSize();
        ImVec2 pts[5];
        pts[0] = pointerBB.Min;
        pts[1] = pointerBB.Min + ImVec2{size.x, 0};
        pts[2] = pointerBB.Min + ImVec2{size.x, size.y * 0.85f};
        pts[3] = pointerBB.Min + ImVec2{size.x / 2, size.y};
        pts[4] = pointerBB.Min + ImVec2{0, size.y * 0.85f};

        //drawList->AddConvexPolyFilled(pts, sizeof(pts) / sizeof(*pts), ImGui::ColorConvertFloat4ToU32({0,1,0,1}));
        drawList->AddConvexPolyFilled(pts, sizeof(pts) / sizeof(*pts), ImGui::ColorConvertFloat4ToU32(topColor));
    }
}

