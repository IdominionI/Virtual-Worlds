#pragma once

#include "node_functions.h"    // Because IMGUI_DEFINE_MATH_OPERATORS no longer valid 
                               // copied and pasted needed functions into this header file

#include <imgui_internal.h>

//  |||||||||||||||| From imnodes.h ||||||||||||||||||

typedef int ImNodesCol;             // -> enum ImNodesCol_
typedef int ImNodesStyleVar;        // -> enum ImNodesStyleVar_
typedef int ImNodesStyleFlags;      // -> enum ImNodesStyleFlags_
typedef int ImNodesPinShape;        // -> enum ImNodesPinShape_
typedef int ImNodesAttributeFlags;  // -> enum ImNodesAttributeFlags_
typedef int ImNodesMiniMapLocation; // -> enum ImNodesMiniMapLocation_


// ------------ STYLE --------------


struct ImNodesColElement
{
    ImU32      Color;
    ImNodesCol Item;

    ImNodesColElement(const ImU32 c, const ImNodesCol s) : Color(c), Item(s) {}
};

enum ImNodesCol_
{
    ImNodesCol_NodeBackground = 0,
    ImNodesCol_NodeBackgroundHovered,
    ImNodesCol_NodeBackgroundSelected,
    ImNodesCol_NodeOutline,
    ImNodesCol_TitleBar,
    ImNodesCol_TitleBarHovered,
    ImNodesCol_TitleBarSelected,
    ImNodesCol_Link,
    ImNodesCol_LinkHovered,
    ImNodesCol_LinkSelected,
    ImNodesCol_Pin,
    ImNodesCol_PinHovered,
    ImNodesCol_BoxSelector,
    ImNodesCol_BoxSelectorOutline,
    ImNodesCol_GridBackground,
    ImNodesCol_GridLine,
    ImNodesCol_MiniMapBackground,
    ImNodesCol_MiniMapBackgroundHovered,
    ImNodesCol_MiniMapOutline,
    ImNodesCol_MiniMapOutlineHovered,
    ImNodesCol_MiniMapNodeBackground,
    ImNodesCol_MiniMapNodeBackgroundHovered,
    ImNodesCol_MiniMapNodeBackgroundSelected,
    ImNodesCol_MiniMapNodeOutline,
    ImNodesCol_MiniMapLink,
    ImNodesCol_MiniMapLinkSelected,
    ImNodesCol_MiniMapCanvas,
    ImNodesCol_MiniMapCanvasOutline,
    ImNodesCol_COUNT
};

enum ImNodesStyleVar_
{
    ImNodesStyleVar_GridSpacing = 0,
    ImNodesStyleVar_NodeCornerRounding,
    ImNodesStyleVar_NodePadding,
    ImNodesStyleVar_NodeBorderThickness,
    ImNodesStyleVar_LinkThickness,
    ImNodesStyleVar_LinkLineSegmentsPerLength,
    ImNodesStyleVar_LinkHoverDistance,
    ImNodesStyleVar_PinCircleRadius,
    ImNodesStyleVar_PinQuadSideLength,
    ImNodesStyleVar_PinTriangleSideLength,
    ImNodesStyleVar_PinLineThickness,
    ImNodesStyleVar_PinHoverRadius,
    ImNodesStyleVar_PinOffset,
    ImNodesStyleVar_MiniMapPadding,
    ImNodesStyleVar_MiniMapOffset,
    ImNodesStyleVar_COUNT
};

enum ImNodesStyleFlags_
{
    ImNodesStyleFlags_None = 0,
    ImNodesStyleFlags_NodeOutline = 1 << 0,
    ImNodesStyleFlags_GridLines = 1 << 2
};

struct ImNodesStyle
{
    float GridSpacing;

    float  NodeCornerRounding;
    ImVec2 NodePadding;
    float  NodeBorderThickness;

    float LinkThickness;
    float LinkLineSegmentsPerLength;
    float LinkHoverDistance;

    // The following variables control the look and behavior of the pins. The default size of each
    // pin shape is balanced to occupy approximately the same surface area on the screen.

    // The circle radius used when the pin shape is either ImNodesPinShape_Circle or
    // ImNodesPinShape_CircleFilled.
    float PinCircleRadius;
    // The quad side length used when the shape is either ImNodesPinShape_Quad or
    // ImNodesPinShape_QuadFilled.
    float PinQuadSideLength;
    // The equilateral triangle side length used when the pin shape is either
    // ImNodesPinShape_Triangle or ImNodesPinShape_TriangleFilled.
    float PinTriangleSideLength;
    // The thickness of the line used when the pin shape is not filled.
    float PinLineThickness;
    // The radius from the pin's center position inside of which it is detected as being hovered
    // over.
    float PinHoverRadius;
    // Offsets the pins' positions from the edge of the node to the outside of the node.
    float PinOffset;

    // Mini-map padding size between mini-map edge and mini-map content.
    ImVec2 MiniMapPadding;
    // Mini-map offset from the screen side.
    ImVec2 MiniMapOffset;

    // By default, ImNodesStyleFlags_NodeOutline and ImNodesStyleFlags_Gridlines are enabled.
    ImNodesStyleFlags Flags;
    // Set these mid-frame using Push/PopColorStyle. You can index this color array with with a
    // ImNodesCol value.
    unsigned int Colors[ImNodesCol_COUNT];

    ImNodesStyle();
};

// ------------ INPUT OUTPUT --------------

struct ImNodesIO
{
    struct EmulateThreeButtonMouse
    {
        EmulateThreeButtonMouse();

        // The keyboard modifier to use in combination with mouse left click to pan the editor view.
        // Set to NULL by default. To enable this feature, set the modifier to point to a boolean
        // indicating the state of a modifier. For example,
        //
        // ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;
        const bool* Modifier;
    } EmulateThreeButtonMouse;

    struct LinkDetachWithModifierClick
    {
        LinkDetachWithModifierClick();

        // Pointer to a boolean value indicating when the desired modifier is pressed. Set to NULL
        // by default. To enable the feature, set the modifier to point to a boolean indicating the
        // state of a modifier. For example,
        //
        // ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;
        //
        // Left-clicking a link with this modifier pressed will detach that link. NOTE: the user has
        // to actually delete the link for this to work. A deleted link can be detected by calling
        // IsLinkDestroyed() after EndNodeEditor().
        const bool* Modifier;
    } LinkDetachWithModifierClick;

    struct MultipleSelectModifier
    {
        MultipleSelectModifier();

        // Pointer to a boolean value indicating when the desired modifier is pressed. Set to NULL
        // by default. To enable the feature, set the modifier to point to a boolean indicating the
        // state of a modifier. For example,
        //
        // ImNodes::GetIO().MultipleSelectModifier.Modifier = &ImGui::GetIO().KeyCtrl;
        //
        // Left-clicking a node with this modifier pressed will add the node to the list of
        // currently selected nodes. If this value is NULL, the Ctrl key will be used.
        const bool* Modifier;
    } MultipleSelectModifier;




    // Holding alt mouse button pans the node area, by default middle mouse button will be used
    // Set based on ImGuiMouseButton values
    int AltMouseButton;

    // Panning speed when dragging an element and mouse is outside the main editor view.
    float AutoPanningSpeed;

    ImNodesIO();
};

ImNodesIO::EmulateThreeButtonMouse::EmulateThreeButtonMouse() : Modifier(NULL) {}

ImNodesIO::LinkDetachWithModifierClick::LinkDetachWithModifierClick() : Modifier(NULL) {}

ImNodesIO::MultipleSelectModifier::MultipleSelectModifier() : Modifier(NULL) {}

ImNodesIO::ImNodesIO()
    : EmulateThreeButtonMouse(), LinkDetachWithModifierClick(),
    AltMouseButton(ImGuiMouseButton_Middle), AutoPanningSpeed(1000.0f)
{
}

ImNodesStyle::ImNodesStyle()
    : GridSpacing(24.f), NodeCornerRounding(4.f), NodePadding(8.f, 8.f), NodeBorderThickness(1.f),
    LinkThickness(3.f), LinkLineSegmentsPerLength(0.1f), LinkHoverDistance(10.f),
    PinCircleRadius(4.f), PinQuadSideLength(7.f), PinTriangleSideLength(9.5),
    PinLineThickness(1.f), PinHoverRadius(10.f), PinOffset(0.f), MiniMapPadding(8.0f, 8.0f),
    MiniMapOffset(4.0f, 4.0f), Flags(ImNodesStyleFlags_NodeOutline | ImNodesStyleFlags_GridLines),
    Colors()
{
}
/*
enum ImNodesMiniMapLocation_
{
    ImNodesMiniMapLocation_BottomLeft,
    ImNodesMiniMapLocation_BottomRight,
    ImNodesMiniMapLocation_TopLeft,
    ImNodesMiniMapLocation_TopRight,
};

#ifndef ImNodesMiniMapNodeHoveringCallback
typedef void (*ImNodesMiniMapNodeHoveringCallback)(int, void*);
#endif

#ifndef ImNodesMiniMapNodeHoveringCallbackUserData
typedef void* ImNodesMiniMapNodeHoveringCallbackUserData;
#endif
*/
// |||||||||||||||| From imnodes_internal.h ||||||||||||||||

// ------------ GRAPH PIN, LINK AND NODE --------------------------

typedef int ImNodesScope;
typedef int ImNodesAttributeType;
typedef int ImNodesUIState;
typedef int ImNodesClickInteractionType;
typedef int ImNodesLinkCreationType;

enum ImNodesScope_
{
    ImNodesScope_None      = 1,
    ImNodesScope_Editor    = 1 << 1,
    ImNodesScope_Node      = 1 << 2,
    ImNodesScope_Attribute = 1 << 3
};

enum ImNodesAttributeType_
{
    ImNodesAttributeType_None,
    ImNodesAttributeType_Input,
    ImNodesAttributeType_Output
};

enum ImNodesUIState_
{
    ImNodesUIState_None = 0,
    ImNodesUIState_LinkStarted = 1 << 0,
    ImNodesUIState_LinkDropped = 1 << 1,
    ImNodesUIState_LinkCreated = 1 << 2
};

enum ImNodesClickInteractionType_
{
    ImNodesClickInteractionType_Node,
    ImNodesClickInteractionType_Link,
    ImNodesClickInteractionType_LinkCreation,
    ImNodesClickInteractionType_Panning,
    ImNodesClickInteractionType_BoxSelection,
    ImNodesClickInteractionType_ImGuiItem,
    ImNodesClickInteractionType_None
};

enum ImNodesLinkCreationType_
{
    ImNodesLinkCreationType_Standard,
    ImNodesLinkCreationType_FromDetach
};

// [SECTION] internal data structures


struct ImNodeData
{
    int    Id;
    ImVec2 Origin; // The node origin is in editor space
    ImRect TitleBarContentRect;
    ImRect Rect;

    struct
    {
        ImU32 Background, BackgroundHovered, BackgroundSelected, Outline, Titlebar, TitlebarHovered,
            TitlebarSelected;
    } ColorStyle;

    struct
    {
        float  CornerRounding;
        ImVec2 Padding;
        float  BorderThickness;
    } LayoutStyle;

    ImVector<int> PinIndices;
    bool          Draggable;

    ImNodeData(const int node_id)
        : Id(node_id), Origin(100.0f, 100.0f), TitleBarContentRect(),
        Rect(ImVec2(0.0f, 0.0f), ImVec2(0.0f, 0.0f)), ColorStyle(), LayoutStyle(), PinIndices(),
        Draggable(true)
    {
    }

    ~ImNodeData() { Id = INT_MIN; }
};

enum ImNodesPinShape_
{
    ImNodesPinShape_Circle,
    ImNodesPinShape_CircleFilled,
    ImNodesPinShape_Triangle,
    ImNodesPinShape_TriangleFilled,
    ImNodesPinShape_Quad,
    ImNodesPinShape_QuadFilled
};

// This enum controls the way the attribute pins behave.
enum ImNodesAttributeFlags_
{
    ImNodesAttributeFlags_None = 0,
    // Allow detaching a link by left-clicking and dragging the link at a pin it is connected to.
    // NOTE: the user has to actually delete the link for this to work. A deleted link can be
    // detected by calling IsLinkDestroyed() after EndNodeEditor().
    ImNodesAttributeFlags_EnableLinkDetachWithDragClick = 1 << 0,
    // Visual snapping of an in progress link will trigger IsLink Created/Destroyed events. Allows
    // for previewing the creation of a link while dragging it across attributes. See here for demo:
    // https://github.com/Nelarius/imnodes/issues/41#issuecomment-647132113 NOTE: the user has to
    // actually delete the link for this to work. A deleted link can be detected by calling
    // IsLinkDestroyed() after EndNodeEditor().
    ImNodesAttributeFlags_EnableLinkCreationOnSnap = 1 << 1
};

struct ImPinData
{
    int                  Id;
    int                  ParentNodeIdx;
    ImRect               AttributeRect;
    ImNodesAttributeType Type;
    ImNodesPinShape      Shape;
    ImVec2               Pos; // screen-space coordinates
    int                  Flags;

    struct
    {
        ImU32 Background, Hovered;
    } ColorStyle;

    ImPinData(const int pin_id)
        : Id(pin_id), ParentNodeIdx(), AttributeRect(), Type(ImNodesAttributeType_None),
        Shape(ImNodesPinShape_CircleFilled), Pos(), Flags(ImNodesAttributeFlags_None),
        ColorStyle()
    {
    }
};

struct ImLinkData
{
    int Id;
    int StartPinIdx, EndPinIdx;

    struct
    {
        ImU32 Base, Hovered, Selected;
    } ColorStyle;

    ImLinkData(const int link_id) : Id(link_id), StartPinIdx(), EndPinIdx(), ColorStyle() {}
};

// Emulates std::optional<int> using the sentinel value `INVALID_INDEX`.
struct ImOptionalIndex
{
    ImOptionalIndex() : _Index(INVALID_INDEX) {}
    ImOptionalIndex(const int value) : _Index(value) {}

    // Observers

    inline bool HasValue() const { return _Index != INVALID_INDEX; }

    inline int Value() const
    {
        assert(HasValue());
        return _Index;
    }

    // Modifiers

    inline ImOptionalIndex& operator=(const int value)
    {
        _Index = value;
        return *this;
    }

    inline void Reset() { _Index = INVALID_INDEX; }

    inline bool operator==(const ImOptionalIndex& rhs) const { return _Index == rhs._Index; }

    inline bool operator==(const int rhs) const { return _Index == rhs; }

    inline bool operator!=(const ImOptionalIndex& rhs) const { return _Index != rhs._Index; }

    inline bool operator!=(const int rhs) const { return _Index != rhs; }

    static const int INVALID_INDEX = -1;

private:
    int _Index;
};

struct ImClickInteractionState
{
    ImNodesClickInteractionType Type;

    struct
    {
        int                     StartPinIdx;
        ImOptionalIndex         EndPinIdx;
        ImNodesLinkCreationType Type;
    } LinkCreation;

    struct
    {
        ImRect Rect; // Coordinates in grid space
    } BoxSelector;

    ImClickInteractionState() : Type(ImNodesClickInteractionType_None) {}
};


struct ImNodesStyleVarElement
{
    ImNodesStyleVar Item;
    float           FloatValue[2];

    ImNodesStyleVarElement(const ImNodesStyleVar variable, const float value) : Item(variable)
    {
        FloatValue[0] = value;
    }

    ImNodesStyleVarElement(const ImNodesStyleVar variable, const ImVec2 value) : Item(variable)
    {
        FloatValue[0] = value.x;
        FloatValue[1] = value.y;
    }
};



// [SECTION] global and editor context structs

// ---------------- CONTEXT DATA STRUCTURES -----------------------


// The object T must have the following interface:
//
// struct T
// {
//     T();
//
//     int id;
// };
template<typename T>
struct ImObjectPool
{
    ImVector<T>    Pool;
    ImVector<bool> InUse;
    ImVector<int>  FreeList;
    ImGuiStorage   IdMap;

    ImObjectPool() : Pool(), InUse(), FreeList(), IdMap() {}
};

struct ImNodesEditorContext
{
    ImObjectPool<ImNodeData> Nodes;
    ImObjectPool<ImPinData>  Pins;
    ImObjectPool<ImLinkData> Links;

    ImVector<int> NodeDepthOrder;

    // ui related fields
    ImVec2 Panning;
    ImVec2 AutoPanningDelta;
    // Minimum and maximum extents of all content in grid space. Valid after final
    // ImNodes::EndNode() call.
    ImRect GridContentBounds;

    ImVector<int> SelectedNodeIndices;
    ImVector<int> SelectedLinkIndices;

    ImClickInteractionState ClickInteraction;
    // BEGIN ADDED TO ORIGINAL CODE!!!!!!!!!!!!!!!!!!!!
// Editor Zoom related variables and parameters

    float zoom_scale = 1.0;

    // Move these to ImNodesStyle structure !!!!!
    float grid_thickness_major = 4.0;
    float grid_thickness_minor = 1.0;
    float grid_spacing_major = 5.0;
    // END ADDED TO ORIGINAL CODE!!!!!!!!!!!!!!!!!!!!
  
    // Mini-map state set by MiniMap() !!!!! Delete as will not use !!!!!!

    //bool                                       MiniMapEnabled;
    //ImNodesMiniMapLocation                     MiniMapLocation;
    //float                                      MiniMapSizeFraction;
    //ImNodesMiniMapNodeHoveringCallback         MiniMapNodeHoveringCallback;
    //ImNodesMiniMapNodeHoveringCallbackUserData MiniMapNodeHoveringCallbackUserData;

    // Mini-map state set during EndNodeEditor() call

    //ImRect MiniMapRectScreenSpace;
    //ImRect MiniMapContentScreenSpace;
    //float  MiniMapScaling;

    //ImNodesEditorContext()
    //    : Nodes(), Pins(), Links(), Panning(0.f, 0.f), SelectedNodeIndices(), SelectedLinkIndices(),
    //    ClickInteraction(), MiniMapEnabled(false), MiniMapSizeFraction(0.0f),
    //    MiniMapNodeHoveringCallback(NULL), MiniMapNodeHoveringCallbackUserData(NULL),
    //    MiniMapScaling(0.0f)
    //{
    //}

    ImNodesEditorContext()
        : Nodes(), Pins(), Links(), Panning(0.f, 0.f), SelectedNodeIndices(), SelectedLinkIndices(),
        ClickInteraction()
    {
    }
};

struct ImNodesContext
{
    ImNodesEditorContext* DefaultEditorCtx;
    ImNodesEditorContext* EditorCtx;

    // Canvas draw list and helper state
    ImDrawList* CanvasDrawList;
    ImGuiStorage  NodeIdxToSubmissionIdx;
    ImVector<int> NodeIdxSubmissionOrder;
    ImVector<int> NodeIndicesOverlappingWithMouse;
    ImVector<int> OccludedPinIndices;

    // Canvas extents
    ImVec2 CanvasOriginScreenSpace;
    ImRect CanvasRectScreenSpace;

    // Debug helpers
    ImNodesScope CurrentScope;

    // Configuration state
    ImNodesIO                        Io;
    ImNodesStyle                     Style;
    ImVector<ImNodesColElement>      ColorModifierStack;
    ImVector<ImNodesStyleVarElement> StyleModifierStack;
    ImGuiTextBuffer                  TextBuffer;

    int           CurrentAttributeFlags;
    ImVector<int> AttributeFlagStack;

    // UI element state
    int CurrentNodeIdx;
    int CurrentPinIdx;
    int CurrentAttributeId;

    ImOptionalIndex HoveredNodeIdx;
    ImOptionalIndex HoveredLinkIdx;
    ImOptionalIndex HoveredPinIdx;

    ImOptionalIndex DeletedLinkIdx;
    ImOptionalIndex SnapLinkIdx;

    // Event helper state
    // TODO: this should be a part of a state machine, and not a member of the global struct.
    // Unclear what parts of the code this relates to.
    int ImNodesUIState;

    int  ActiveAttributeId;
    bool ActiveAttribute;

    // ImGui::IO cache

    ImVec2 MousePos;

    bool  LeftMouseClicked;
    bool  LeftMouseReleased;
    bool  AltMouseClicked;
    bool  LeftMouseDragging;
    bool  AltMouseDragging;
    float AltMouseScrollDelta;
};

