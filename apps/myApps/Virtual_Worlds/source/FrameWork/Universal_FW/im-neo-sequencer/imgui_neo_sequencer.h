#pragma once

//
// Created by Matty on 2022-01-28.
//

#include <vector>

#include "imgui.h"

#include "imgui_neo_internal.h"

typedef int32_t FrameIndexType;

typedef int ImGuiNeoSequencerFlags;
typedef int ImGuiNeoSequencerCol;
typedef int ImGuiNeoTimelineFlags;
typedef int ImGuiNeoTimelineIsSelectedFlags;

// Flags for ImGui::BeginNeoSequencer()
enum ImGuiNeoSequencerFlags_
{
    ImGuiNeoSequencerFlags_None                 = 0     ,
    ImGuiNeoSequencerFlags_AllowLengthChanging  = 1 << 0, // Allows changing length of sequence
    ImGuiNeoSequencerFlags_EnableSelection      = 1 << 1, // Enables selection of keyframes
    ImGuiNeoSequencerFlags_HideZoom             = 1 << 2, // Disables zoom bar
    //ImGuiNeoSequencerFlags_PH                 = 1 << 3, // PLACEHOLDER
    ImGuiNeoSequencerFlags_AlwaysShowHeader     = 1 << 4, // Enables overlay header, keeping it visible when scrolling

    // Selection options, only work with enable selection flag
    ImGuiNeoSequencerFlags_Selection_EnableDragging = 1 << 5,
    ImGuiNeoSequencerFlags_Selection_EnableDeletion = 1 << 6,
    
    ImGuiNeoSequencerFlags_Enable_controls          = 1 << 7,

};

// Flags for ImGui::BeginNeoTimeline()
enum ImGuiNeoTimelineFlags_
{
    ImGuiNeoTimelineFlags_None                 = 0     ,
    ImGuiNeoTimelineFlags_AllowFrameChanging   = 1 << 0,
    ImGuiNeoTimelineFlags_Group                = 1 << 1,
};

// Flags for ImGui::IsNeoTimelineSelected()
enum ImGuiNeoTimelineIsSelectedFlags_
{
    ImGuiNeoTimelineIsSelectedFlags_None            = 0     ,
    ImGuiNeoTimelineIsSelectedFlags_NewlySelected   = 1 << 0,
};

enum ImGuiNeoSequencerCol_
{
    ImGuiNeoSequencerCol_Bg,
    ImGuiNeoSequencerCol_TopBarBg,
    ImGuiNeoSequencerCol_SelectedTimeline,
    ImGuiNeoSequencerCol_TimelineBorder,
    ImGuiNeoSequencerCol_TimelinesBg,
    ImGuiNeoSequencerCol_FramePointer,
    ImGuiNeoSequencerCol_FramePointerHovered,
    ImGuiNeoSequencerCol_FramePointerPressed,
    ImGuiNeoSequencerCol_Keyframe,
    ImGuiNeoSequencerCol_KeyframeHovered,
    ImGuiNeoSequencerCol_KeyframePressed,
    ImGuiNeoSequencerCol_KeyframeSelected,
    ImGuiNeoSequencerCol_FramePointerLine,

    ImGuiNeoSequencerCol_ZoomBarBg,
    ImGuiNeoSequencerCol_ZoomBarSlider,
    ImGuiNeoSequencerCol_ZoomBarSliderHovered,
    ImGuiNeoSequencerCol_ZoomBarSliderEnds,
    ImGuiNeoSequencerCol_ZoomBarSliderEndsHovered,

    ImGuiNeoSequencerCol_SelectionBorder,
    ImGuiNeoSequencerCol_Selection,

    ImGuiNeoSequencerCol_COUNT
};

struct ImGuiNeoSequencerStyle {
    float       SequencerRounding       = 2.5f;                 // Corner rounding around whole sequencer
    float       TopBarHeight            = 0.0f;                 // Value <= 0.0f = Height is calculated by FontSize + FramePadding.y * 2.0f
    bool        TopBarShowFrameLines    = true;                 // Show line for every frame in top bar
    bool        TopBarShowFrameTexts    = true;                 // Show frame number every 10th frame
    ImVec2      ItemSpacing             = {4.0f,0.5f};
    float       DepthItemSpacing        = 10.0f;                // Amount of text offset per depth level in timeline values
    float       TopBarSpacing           = 3.0f;                 // Space between top bar and timeline
    float       TimelineBorderSize      = 1.0f;
    float       CurrentFramePointerSize = 7.0f;                 // Size of pointing arrow above current frame line
    float       CurrentFrameLineWidth   = 1.0f;                 // Width of line showing current frame over timeline
    float       ZoomHeightScale         = 1.0f;                 // Scale of Zoom bar, base height is font size
    float       CollidedKeyframeOffset  = 3.5f;                 // Offset on which colliding keyframes are rendered

    float       MaxSizePerTick          = 4.0f;                 // Maximum amount of pixels per tick on timeline (if less pixels is present, ticks are not rendered)

    ImVec4      Colors[ImGuiNeoSequencerCol_COUNT];

    ImGuiKey    ModRemoveKey            = ImGuiMod_Ctrl;        // Key mod which when held removes selected keyframes from present selection
    ImGuiKey    ModAddKey               = ImGuiMod_Shift;       // Key mod which when held adds selected keyframes to present selection

    ImGuiNeoSequencerStyle();
};

// Internal state, used for deletion of old keyframes.
struct ImGuiNeoTimelineKeyframes
{
    ImGuiID TimelineID;
    ImVector<int32_t> KeyframesToDelete;
};

// Internal struct holding how many times was keyframe on certain frame rendered, used as offset for duplicates
struct ImGuiNeoKeyframeDuplicate
{
    int32_t Frame;
    uint32_t Count;
};

enum class SelectionState
{
    Idle, // Doing nothing related
    Selecting,  // Selecting selection
    Dragging    // Dragging selection
};

struct ImGuiNeoSequencerInternalData
{
    float header_display_offset = 0.0f;  // y Offset from top of window to allow additional widgets or graphics to be displayed above sequencer main body.
    float frame_display_padding = 50.0f; // x Padding of frame display from edge of window. To allow display of end frame 

    ImVec2 TopLeftCursor = { 0, 0 };   // Cursor on top of whole widget
    ImVec2 TopBarStartCursor = { 0, 0 }; // Cursor on top, below Zoom slider
    ImVec2 StartValuesCursor = { 0, 0 }; // Cursor on top of values
    ImVec2 ValuesCursor = { 0, 0 }; // Current cursor position, used for values drawing

    ImVec2 Size = { 0, 0 }; // Size of whole sequencer
    ImVec2 TopBarSize = { 0, 0 }; // Size of top bar without Zoom

    FrameIndexType StartFrame = 0;
    FrameIndexType EndFrame = 0;
    FrameIndexType OffsetFrame = 0; // Offset from start

    //float ValuesWidth = 32.0f; // Width of biggest label in timeline, used for offset of timeline
    float ValuesWidth = 48.0f; // Width of biggest label in timeline, used for offset of timeline

    float FilledHeight = 0.0f; // Height of whole sequencer

    float Zoom = 1.0f;

    ImGuiID Id;

    ImGuiID LastSelectedTimeline = 0;
    ImGuiID SelectedTimeline = 0;
    bool LastTimelineOpenned = false;

    ImVector<ImGuiID> TimelineStack;
    ImVector<ImGuiID> GroupStack;

    FrameIndexType CurrentFrame = 0;
    bool HoldingCurrentFrame = false; // Are we draging current frame?
    ImVec4 CurrentFrameColor; // Color of current frame, we have to save it because we render on EndNeoSequencer, but process at BeginneoSequencer

    bool HoldingZoomSlider = false;

    //Selection
    ImVector<ImGuiID> Selection; // Contains ids of keyframes
    ImVec2 SelectionMouseStart = { 0, 0 };
    SelectionState StateOfSelection = SelectionState::Idle;
    ImVec2 DraggingMouseStart = { 0, 0 };
    bool StartDragging = true;
    ImVector<int32_t> DraggingSelectionStart; // Contains start values of all selection elements
    bool DraggingEnabled = true;
    bool SelectionEnabled = true;
    bool IsSelectionRightClicked = false;

    //Last keyframe data
    bool IsLastKeyframeHovered = false;
    bool IsLastKeyframeSelected = false;
    bool IsLastKeyframeRightClicked = false;

    //Deletion
    bool DeleteDataDirty = false;
    bool DeleteEnabled = true;
    ImVector<ImGuiNeoTimelineKeyframes> SelectionData;
};

ImGuiNeoSequencerStyle::ImGuiNeoSequencerStyle()
{
    Colors[ImGuiNeoSequencerCol_Bg] = ImVec4{0.31f, 0.31f, 0.31f, 1.00f};
    Colors[ImGuiNeoSequencerCol_TopBarBg] = ImVec4{0.22f, 0.22f, 0.22f, 0.84f};
    Colors[ImGuiNeoSequencerCol_SelectedTimeline] = ImVec4{0.98f, 0.706f, 0.322f, 0.88f};
    Colors[ImGuiNeoSequencerCol_TimelinesBg] = Colors[ImGuiNeoSequencerCol_TopBarBg];
    Colors[ImGuiNeoSequencerCol_TimelineBorder] = Colors[ImGuiNeoSequencerCol_Bg] * ImVec4{0.5f, 0.5f, 0.5f, 1.0f};

    Colors[ImGuiNeoSequencerCol_FramePointer] = ImVec4{0.98f, 0.24f, 0.24f, 0.50f};
    Colors[ImGuiNeoSequencerCol_FramePointerHovered] = ImVec4{0.98f, 0.15f, 0.15f, 1.00f};
    Colors[ImGuiNeoSequencerCol_FramePointerPressed] = ImVec4{0.98f, 0.08f, 0.08f, 1.00f};

    Colors[ImGuiNeoSequencerCol_Keyframe] = ImVec4{0.59f, 0.59f, 0.59f, 0.50f};
    Colors[ImGuiNeoSequencerCol_KeyframeHovered] = ImVec4{0.98f, 0.39f, 0.36f, 1.00f};
    Colors[ImGuiNeoSequencerCol_KeyframePressed] = ImVec4{0.98f, 0.39f, 0.36f, 1.00f};
    Colors[ImGuiNeoSequencerCol_KeyframeSelected] = ImVec4{0.32f, 0.23f, 0.98f, 1.00f};

    Colors[ImGuiNeoSequencerCol_FramePointerLine] = ImVec4{0.98f, 0.98f, 0.98f, 0.8f};

    Colors[ImGuiNeoSequencerCol_ZoomBarBg] = ImVec4{0.59f, 0.59f, 0.59f, 0.90f};
    Colors[ImGuiNeoSequencerCol_ZoomBarSlider] = ImVec4{0.8f, 0.8f, 0.8f, 0.60f};
    Colors[ImGuiNeoSequencerCol_ZoomBarSliderHovered] = ImVec4{0.98f, 0.98f, 0.98f, 0.80f};
    Colors[ImGuiNeoSequencerCol_ZoomBarSliderEnds] = ImVec4{0.59f, 0.59f, 0.59f, 0.90f};
    Colors[ImGuiNeoSequencerCol_ZoomBarSliderEndsHovered] = ImVec4{0.93f, 0.93f, 0.93f, 0.93f};

    Colors[ImGuiNeoSequencerCol_SelectionBorder] = ImVec4{0.98f, 0.706f, 0.322f, 0.61f};
    Colors[ImGuiNeoSequencerCol_Selection] = ImVec4{0.98f, 0.706f, 0.322f, 0.33f};

}

class neo_sequencer_class : public neo_sequencer_internal_class {
public:
    uint32_t idCounter = 0;
    char idBuffer[16];


    ImGuiNeoSequencerStyle style; // NOLINT(cert-err58-cpp)

    //Global context stuff
    bool inSequencer = false;

    // Height of timeline right now
    float currentTimelineHeight = 0.0f;

    // Current active sequencer
    ImGuiID currentSequencer;

    // Current timeline depth, used for offset of label
    uint32_t currentTimelineDepth = 0;

    ImVector<ImGuiColorMod> sequencerColorStack;

    // Data of all sequencers, this is main c++ part and I should create C alternative or use imgui ImVector or something
    //std::unordered_map<ImGuiID, ImGuiNeoSequencerInternalData> sequencerData;
    ImGuiNeoSequencerInternalData sequencerData;

    ImVector<ImGuiNeoKeyframeDuplicate> keyframeDuplicates;

    // ******
    float  getPerFrameWidth(ImGuiNeoSequencerInternalData& context);
    float  getKeyframePositionX(FrameIndexType frame, ImGuiNeoSequencerInternalData& context);
    float  getWorkTimelineWidth(ImGuiNeoSequencerInternalData& context);
    ImRect getCurrentFrameBB(FrameIndexType frame, ImGuiNeoSequencerInternalData& context);
    void   processCurrentFrame(FrameIndexType* frame, ImGuiNeoSequencerInternalData& context);
    void   finishPreviousTimeline(ImGuiNeoSequencerInternalData& context);
    ImColor getKeyframeColor(ImGuiNeoSequencerInternalData& context, bool hovered, bool inSelection);
    void   addKeyframeToDeleteData(int32_t value, ImGuiNeoSequencerInternalData& context, const ImGuiID timelineId);
    bool   getKeyframeInSelection(int32_t value, ImGuiID id, ImGuiNeoSequencerInternalData& context, const ImRect bb);
    ImGuiID getKeyframeID(int32_t* frame);
    bool createKeyframe(int32_t* frame);
    const char* generateID();
    void        resetID();
    void renderCurrentFrame(ImGuiNeoSequencerInternalData& context);
    
    float calculateZoomBarHeight();

    void  processAndRenderZoom(ImGuiNeoSequencerInternalData& context, const ImVec2& cursor, bool allowEditingLength,
                               FrameIndexType* start, FrameIndexType* end);
    
    void processSelection(ImGuiNeoSequencerInternalData& context);
    void renderSelection(ImGuiNeoSequencerInternalData& context);
    bool groupBehaviour(const ImGuiID id, bool* open, const ImVec2 labelSize);
    bool timelineBehaviour(const ImGuiID id, const ImVec2 labelSize);


    // ******

    const ImVec4& GetStyleNeoSequencerColorVec4(ImGuiNeoSequencerCol idx);
    ImGuiNeoSequencerStyle& GetNeoSequencerStyle();

    void PushNeoSequencerStyleColor(ImGuiNeoSequencerCol idx, ImU32 col);
    void PushNeoSequencerStyleColor(ImGuiNeoSequencerCol idx, const ImVec4& col);
    void PopNeoSequencerStyleColor(int count = 1);

    bool BeginNeoSequencer(const char* id, FrameIndexType* frame, FrameIndexType* startFrame, FrameIndexType* endFrame, const ImVec2& size = ImVec2(0, 0), ImGuiNeoSequencerFlags flags = ImGuiNeoSequencerFlags_None);
    void EndNeoSequencer(); //Call only when BeginNeoSequencer() returns true!!

    bool BeginNeoGroup(const char* label, bool* open = nullptr);
    void EndNeoGroup();

    bool BeginNeoTimeline(const char* label, FrameIndexType** keyframes, uint32_t keyframeCount, bool* open = nullptr, ImGuiNeoTimelineFlags flags = ImGuiNeoTimelineFlags_None);
    void EndNeoTimeLine(); //Call only when BeginNeoTimeline() returns true!!

    // Fully customizable timeline with per key callback
    bool BeginNeoTimelineEx(const char* label, bool* open = nullptr, ImGuiNeoTimelineFlags flags = ImGuiNeoTimelineFlags_None);
    void NeoKeyframe(int32_t* value);

    bool IsNeoKeyframeHovered();
    bool IsNeoKeyframeSelected();
    bool IsNeoKeyframeRightClicked();


    // Selection API
    // DON'T delete keyframes while dragging, internal buffer will get corrupted
    // Order for deletion is generally:
    // CanDelete? -> DataSize? -> GetData() -> Delete your data -> ClearSelection()
    void NeoClearSelection(); // Clears selection
    bool NeoIsSelecting(); // Are we currently selecting?
    bool NeoHasSelection(); // Is anything selected?
    bool NeoIsDraggingSelection(); // Are we dragging selection?
    bool NeoCanDeleteSelection(); // Can selection deletion be done?
    bool IsNeoKeyframeSelectionRightClicked(); // Is selection rightclicked?

    // Call only in BeginNeoTimeline / EndNeoTimeLine scope, returns selection per timeline and size per timeline
    uint32_t GetNeoKeyframeSelectionSize();
    void GetNeoKeyframeSelection(FrameIndexType* selection);


    // Sets currently selected timeline inside BeginNeoSequencer scope
    void SetSelectedTimeline(const char* timelineLabel);

    bool IsNeoTimelineSelected(ImGuiNeoTimelineIsSelectedFlags flags = ImGuiNeoTimelineIsSelectedFlags_None);

#ifdef __cplusplus
    // C++ helper
    bool BeginNeoTimeline(const char* label, std::vector<int32_t>& keyframes, bool* open = nullptr, ImGuiNeoTimelineFlags flags = ImGuiNeoTimelineFlags_None);
#endif

};
