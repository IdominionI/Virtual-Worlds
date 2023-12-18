#pragma once
#include "graph.h"

#include "node_editor_parameters.h"

#include <imgui.h>

struct node_editor_style_struct_type {
    ImColor default_node_title_bar_color;// ImColor(0, 139, 139, 255));
    ImColor default_node_title_bar_hovered_color;// ImColor(0, 139, 139, 255));
    ImColor default_node_title_bar_selected_color;// ImColor(0, 139, 139, 255));
    ImColor default_node_title_bar_label_color;// ImColor(0, 139, 139, 255));
    std::string default_node_title_bar_label_font;// ImColor(0, 139, 139, 255));
    int default_node_title_bar_label_size;// ImColor(0, 139, 139, 255));
    
    
    ImColor default_pin_color;// ImColor(0, 139, 139, 255));
    ImColor default_pin_hover_color;// ImColor(0, 139, 139, 255));
    ImColor default_pin_label_color;// ImColor(0, 139, 139, 255));
    std::string default_pin_label_font;// ImColor(0, 139, 139, 255));
    int     default_pin_label_size;
    ImNodesPinShape_ default_pin_shape = ImNodesPinShape_::ImNodesPinShape_QuadFilled;
    
    ImColor default_link_color;// ImColor(0, 139, 139, 255));
    ImColor default_link_hover_color;// ImColor(0, 139, 139, 255));
    ImColor default_link_selected_color;// ImColor(0, 139, 139, 255));
    float   default_link_thickness = 1.0;
    float   default_link_line_segments_per_length = 1.0;
    //graph->node_editor->PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 139, 139, 255));
    //graph->node_editor->PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 175, 175, 255));
    //graph->node_editor->PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 255, 255));

};

class node_editor_basis_class {
public:
    ImNodesContext GImNodes;

    node_editor_basis_class() {
        Initialize();
    }

    ~node_editor_basis_class() {

    }

    graph_class graph;
 
protected:
    //graph_class graph;
    int         root_node_id_;

    bool no_node_element_is_hovered_over = true;

    int hover_node_id, hover_link_id, hover_pin_id;
    int current_selected_node_id = -1;
    int num_selected_nodes = 0;

    int current_selected_link_id = -1;//*****
    int num_selected_links = 0;//*****

    bool inside_editor_canvas(ImVec2 location) {
        if (location.x >= GImNodes.CanvasRectScreenSpace.Min.x &&
            location.x <= GImNodes.CanvasRectScreenSpace.Max.x &&
            location.y >= GImNodes.CanvasRectScreenSpace.Min.y &&
            location.y <= GImNodes.CanvasRectScreenSpace.Max.y)
            return true;
        else
            return false;
    }

    bool no_element_hovered_over() {
        if (IsNodeHovered(&hover_node_id) ||
            IsLinkHovered(&hover_link_id) ||
            IsPinHovered(&hover_pin_id))
            return false;
        else
            return true;
    }

// !!!!!!!!!!!!!!!! BEGIN CLASS MANAGEMENT SECTION !!!!!!!!!!!!!!!!!!!!!!!!!!

    //void Initialize(ImNodesContext *context)
    void Initialize()
    {
        GImNodes.CanvasOriginScreenSpace = ImVec2(0.0f, 0.0f);
        GImNodes.CanvasRectScreenSpace = ImRect(ImVec2(0.f, 0.f), ImVec2(0.f, 0.f));
        GImNodes.CurrentScope = ImNodesScope_None;

        GImNodes.CurrentPinIdx = INT_MAX;
        GImNodes.CurrentNodeIdx = INT_MAX;

        GImNodes.DefaultEditorCtx = EditorContextCreate();
        EditorContextSet(GImNodes.DefaultEditorCtx);

        GImNodes.CurrentAttributeFlags = ImNodesAttributeFlags_None;
        GImNodes.AttributeFlagStack.push_back(GImNodes.CurrentAttributeFlags);

        StyleColorsDark();

        graph.node_editor = this;
    }

    void Shutdown() {
        EditorContextFree(GImNodes.DefaultEditorCtx); 
        EditorContextFree(GImNodes.EditorCtx); 
    }

private:
    ImNodesEditorContext *EditorContextCreate()
    {
        void* mem = ImGui::MemAlloc(sizeof(ImNodesEditorContext));
        new (mem) ImNodesEditorContext();
        return (ImNodesEditorContext*)mem;
    }

    void EditorContextFree(ImNodesEditorContext* ctx)
    {
        ctx->~ImNodesEditorContext();
        ImGui::MemFree(ctx);
    }

// !!!!!!!!!!!!!!!! END CLASS MANAGEMENT SECTION !!!!!!!!!!!!!!!!!!!!!!!!!!

// !!!!!!!!!!!!!!!! BEGIN EDITOR MANAGEMENT SECTION !!!!!!!!!!!!!!!!!!!!!!!!!!
// Determine what is needed and what is not or even if begin_node_editor() and end_node_editor() is required for this class
protected:
    void BeginNodeEditor() {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        GImNodes.CurrentScope = ImNodesScope_Editor;

        // Reset state from previous pass

        ImNodesEditorContext *editor = GImNodes.EditorCtx;

        editor->AutoPanningDelta = ImVec2(0, 0);
        editor->GridContentBounds = ImRect(FLT_MAX, FLT_MAX, FLT_MIN, FLT_MIN);
        //editor->MiniMapEnabled = false;
        ObjectPoolReset(editor->Nodes);
        ObjectPoolReset(editor->Pins);
        ObjectPoolReset(editor->Links);

        GImNodes.HoveredNodeIdx.Reset();
        GImNodes.HoveredLinkIdx.Reset();
        GImNodes.HoveredPinIdx.Reset();
        GImNodes.DeletedLinkIdx.Reset();
        GImNodes.SnapLinkIdx.Reset();

        GImNodes.NodeIndicesOverlappingWithMouse.clear();

        GImNodes.ImNodesUIState = ImNodesUIState_None;

        GImNodes.MousePos = ImGui::GetIO().MousePos;
        GImNodes.LeftMouseClicked = ImGui::IsMouseClicked(0);
        GImNodes.LeftMouseReleased = ImGui::IsMouseReleased(0);
        GImNodes.AltMouseClicked =
            (GImNodes.Io.EmulateThreeButtonMouse.Modifier != NULL &&
                *GImNodes.Io.EmulateThreeButtonMouse.Modifier && GImNodes.LeftMouseClicked) ||
            ImGui::IsMouseClicked(GImNodes.Io.AltMouseButton);
        GImNodes.LeftMouseDragging = ImGui::IsMouseDragging(0, 0.0f);
        GImNodes.AltMouseDragging =
            (GImNodes.Io.EmulateThreeButtonMouse.Modifier != NULL && GImNodes.LeftMouseDragging &&
                (*GImNodes.Io.EmulateThreeButtonMouse.Modifier)) ||
            ImGui::IsMouseDragging(GImNodes.Io.AltMouseButton, 0.0f);
        GImNodes.AltMouseScrollDelta = ImGui::GetIO().MouseWheel;

        GImNodes.ActiveAttribute = false;

        ImGui::BeginGroup();
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.f, 1.f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
            ImGui::PushStyleColor(ImGuiCol_ChildBg, GImNodes.Style.Colors[ImNodesCol_GridBackground]);
            ImGui::BeginChild(
                "scrolling_region",
                ImVec2(0.f, 0.f),
                true,
                ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoScrollWithMouse);
            GImNodes.CanvasOriginScreenSpace = ImGui::GetCursorScreenPos();

            // NOTE: we have to fetch the canvas draw list *after* we call
            // BeginChild(), otherwise the ImGui UI elements are going to be
            // rendered into the parent window draw list.
            DrawListSet(ImGui::GetWindowDrawList());

            {
                const ImVec2 canvas_size = ImGui::GetWindowSize();
                GImNodes.CanvasRectScreenSpace = ImRect(
                    EditorSpaceToScreenSpace(ImVec2(0.f, 0.f)), EditorSpaceToScreenSpace(canvas_size));

                if (GImNodes.Style.Flags & ImNodesStyleFlags_GridLines)
                {
                    DrawGrid(editor, canvas_size);
                }
            }
        }
    }

    void EndNodeEditor() {
//std::cout << "node_editor_basis_class :: EndNodeEditor  AAAAA\n";
        assert(GImNodes.CurrentScope == ImNodesScope_Editor);
        GImNodes.CurrentScope = ImNodesScope_None;

        ImNodesEditorContext *editor = GImNodes.EditorCtx;

        bool no_grid_content = editor->GridContentBounds.IsInverted();
        if (no_grid_content){
            editor->GridContentBounds = ScreenSpaceToGridSpace(GImNodes.CanvasRectScreenSpace);
            //editor->GridContentBounds = ScreenSpaceToGridSpace(editor, GImNodes.CanvasRectScreenSpace);
        }

        // Detect ImGui interaction first, because it blocks interaction with the rest of the UI

        if (GImNodes.LeftMouseClicked && ImGui::IsAnyItemActive())
        {
            editor->ClickInteraction.Type = ImNodesClickInteractionType_ImGuiItem;
        }
//std::cout << "node_editor_basis_class :: EndNodeEditor  BBBBB\n";
        // Detect which UI element is being hovered over. Detection is done in a hierarchical fashion,
        // because a UI element being hovered excludes any other as being hovered over.

        // Don't do hovering detection for nodes/links/pins when interacting with the mini-map, since
        // its an *overlay* with its own interaction behavior and must have precedence during mouse
        // interaction.

        if ((editor->ClickInteraction.Type == ImNodesClickInteractionType_None ||
            editor->ClickInteraction.Type == ImNodesClickInteractionType_LinkCreation) &&
            MouseInCanvas())
            //MouseInCanvas() && !IsMiniMapHovered()) // No minimap to be implemented
        {
            // Pins needs some special care. We need to check the depth stack to see which pins are
            // being occluded by other nodes.
            ResolveOccludedPins(editor, GImNodes.OccludedPinIndices);

            GImNodes.HoveredPinIdx = ResolveHoveredPin(editor->Pins, GImNodes.OccludedPinIndices);

            if (!GImNodes.HoveredPinIdx.HasValue())
            {
                // Resolve which node is actually on top and being hovered using the depth stack.
                GImNodes.HoveredNodeIdx = ResolveHoveredNode(editor->NodeDepthOrder);
            }

            // We don't check for hovered pins here, because if we want to detach a link by clicking and
            // dragging, we need to have both a link and pin hovered.
            if (!GImNodes.HoveredNodeIdx.HasValue())
            {
                GImNodes.HoveredLinkIdx = ResolveHoveredLink(editor->Links, editor->Pins);
            }
        }
//std::cout << "node_editor_basis_class :: EndNodeEditor  CCCCC : " << editor->Nodes.Pool.size() << std::endl;
        for (int node_idx = 0; node_idx < editor->Nodes.Pool.size(); ++node_idx)
        {
//std::cout << "node_editor_basis_class :: EndNodeEditor  CCCCC11111 : " << node_idx << ":" << editor->Nodes.InUse.size() << std::endl;
            if (editor->Nodes.InUse[node_idx])
            {
//std::cout << "node_editor_basis_class :: EndNodeEditor  CCCCC2222 : \n" ;
                DrawListActivateNodeBackground(node_idx);
//std::cout << "node_editor_basis_class :: EndNodeEditor  CCCCC3333\n";
                DrawNode(editor, node_idx);
            
            }
        }
//std::cout << "node_editor_basis_class :: EndNodeEditor  DDDDDD\n";
        // In order to render the links underneath the nodes, we want to first select the bottom draw
        // channel.
        GImNodes.CanvasDrawList->ChannelsSetCurrent(0);
//std::cout << "node_editor_basis_class :: EndNodeEditor  EEEEE\n";
        for (int link_idx = 0; link_idx < editor->Links.Pool.size(); ++link_idx)
        {
            if (editor->Links.InUse[link_idx])
            {
                DrawLink(editor, link_idx);
            }
        }

        // Render the click interaction UI elements (partial links, box selector) on top of everything
        // else.
//std::cout << "node_editor_basis_class :: EndNodeEditor  DDDDDD\n";
        DrawListAppendClickInteractionChannel();
        DrawListActivateClickInteractionChannel();

        //if (IsMiniMapActive())
        //{
        //    CalcMiniMapLayout();
        //    MiniMapUpdate();
        //}

        // Handle node graph interaction

        //if (!IsMiniMapHovered())
        //{
            if (GImNodes.LeftMouseClicked && GImNodes.HoveredLinkIdx.HasValue())
            {
                BeginLinkInteraction(editor, GImNodes.HoveredLinkIdx.Value(), GImNodes.HoveredPinIdx);
            }

            else if (GImNodes.LeftMouseClicked && GImNodes.HoveredPinIdx.HasValue())
            {
                BeginLinkCreation(editor, GImNodes.HoveredPinIdx.Value());
            }

            else if (GImNodes.LeftMouseClicked && GImNodes.HoveredNodeIdx.HasValue())
            {
                BeginNodeSelection(editor, GImNodes.HoveredNodeIdx.Value());
            }

            else if (
                GImNodes.LeftMouseClicked || GImNodes.LeftMouseReleased ||
                GImNodes.AltMouseClicked || GImNodes.AltMouseScrollDelta != 0.f)
            {
                BeginCanvasInteraction(editor);
            }

            bool should_auto_pan =
                editor->ClickInteraction.Type == ImNodesClickInteractionType_BoxSelection ||
                editor->ClickInteraction.Type == ImNodesClickInteractionType_LinkCreation ||
                editor->ClickInteraction.Type == ImNodesClickInteractionType_Node;
            if (should_auto_pan && !MouseInCanvas())
            {
                ImVec2 mouse = ImGui::GetMousePos();
                ImVec2 center = GImNodes.CanvasRectScreenSpace.GetCenter();
                ImVec2 direction = (center - mouse);
                direction = direction * ImInvLength(direction, 0.0);

                editor->AutoPanningDelta =
                    direction * ImGui::GetIO().DeltaTime * GImNodes.Io.AutoPanningSpeed;
                editor->Panning += editor->AutoPanningDelta;
            }
        //}
        ClickInteractionUpdate(editor);
//std::cout << "node_editor_basis_class :: EndNodeEditor  EEEEE\n";
        // At this point, draw commands have been issued for all nodes (and pins). Update the node pool
        // to detect unused node slots and remove those indices from the depth stack before sorting the
        // node draw commands by depth.
        ObjectPoolUpdate(editor->Nodes);
        ObjectPoolUpdate(editor->Pins);

        DrawListSortChannelsByDepth(editor->NodeDepthOrder);

        // After the links have been rendered, the link pool can be updated as well.
        ObjectPoolUpdate(editor->Links);

        // Finally, merge the draw channels
        GImNodes.CanvasDrawList->ChannelsMerge();

        // pop style
        ImGui::EndChild();      // end scrolling region
        ImGui::PopStyleColor(); // pop child window background color
        ImGui::PopStyleVar();   // pop window padding
        ImGui::PopStyleVar();   // pop frame padding
        ImGui::EndGroup();

//std::cout << "node_editor_basis_class :: EndNodeEditor ZZZZ \n";
    }

    void EditorContextSet(ImNodesEditorContext* ctx) { GImNodes.EditorCtx = ctx; }
public:
    ImNodesEditorContext * EditorContextGet()
    {
        // No editor context was set! Did you forget to call ImNodes::CreateContext()?
        assert(GImNodes.EditorCtx != NULL);
        return GImNodes.EditorCtx;
    }

    ImVec2 EditorContextGetPanning()
    {
        const ImNodesEditorContext  *editor = GImNodes.EditorCtx;
        return editor->Panning;
    }

    void EditorContextResetPanning(const ImVec2& pos)
    {
        ImNodesEditorContext *editor = EditorContextGet();
        editor->Panning = pos;
    }

    void EditorContextMoveToNode(const int node_id)
    {
        ImNodesEditorContext *editor = EditorContextGet();
        ImNodeData& node = ObjectPoolFindOrCreateObject(editor->Nodes, node_id);

        editor->Panning.x = -node.Origin.x;
        editor->Panning.y = -node.Origin.y;
    }

    void SetImGuiContext(ImGuiContext* ctx) { ImGui::SetCurrentContext(ctx); }

    ImNodesIO& GetIO() { return GImNodes.Io; }

    ImNodesStyle& GetStyle() { return GImNodes.Style; }

// !!!!!!!!!!!!!!!! END EDITOR MANAGEMENT SECTION !!!!!!!!!!!!!!!!!!!!!!!!!!

    // [SECTION] coordinate space conversion helpers

    //inline ImVec2 ScreenSpaceToGridSpace(const ImVec2& v){
    ImVec2 ScreenSpaceToGridSpace(const ImVec2& v){
        return v - GImNodes.CanvasOriginScreenSpace - GImNodes.EditorCtx->Panning;
    }

    //inline ImRect ScreenSpaceToGridSpace(const ImRect& r){
    ImRect ScreenSpaceToGridSpace(const ImRect& r){
        return ImRect(ScreenSpaceToGridSpace(r.Min), ScreenSpaceToGridSpace(r.Max));
    }

    //inline ImVec2 GridSpaceToScreenSpace(const ImVec2& v){
    ImVec2 GridSpaceToScreenSpace(const ImVec2& v){
        return v + GImNodes.CanvasOriginScreenSpace + GImNodes.EditorCtx->Panning;
    }

    //inline ImVec2 GridSpaceToEditorSpace(const ImVec2& v){
    ImVec2 GridSpaceToEditorSpace(const ImVec2& v){
        return v + GImNodes.EditorCtx->Panning;
    }

    //inline ImVec2 EditorSpaceToGridSpace(const ImVec2& v){
    ImVec2 EditorSpaceToGridSpace(const ImVec2& v){
        return v - GImNodes.EditorCtx->Panning;
    }

    //inline ImVec2 EditorSpaceToScreenSpace(const ImVec2& v){
    ImVec2 EditorSpaceToScreenSpace(const ImVec2& v){
        return GImNodes.CanvasOriginScreenSpace + v;
    }


    // [SECTION] draw list helper
private:
    void ImDrawListGrowChannels(ImDrawList* draw_list, const int num_channels)
    {
        ImDrawListSplitter& splitter = draw_list->_Splitter;

        if (splitter._Count == 1)
        {
            splitter.Split(draw_list, num_channels + 1);
            return;
        }

        // NOTE: this logic has been lifted from ImDrawListSplitter::Split with slight modifications
        // to allow nested splits. The main modification is that we only create new ImDrawChannel
        // instances after splitter._Count, instead of over the whole splitter._Channels array like
        // the regular ImDrawListSplitter::Split method does.

        const int old_channel_capacity = splitter._Channels.Size;
        // NOTE: _Channels is not resized down, and therefore _Count <= _Channels.size()!
        const int old_channel_count = splitter._Count;
        const int requested_channel_count = old_channel_count + num_channels;
        if (old_channel_capacity < old_channel_count + num_channels)
        {
            splitter._Channels.resize(requested_channel_count);
        }

        splitter._Count = requested_channel_count;

        for (int i = old_channel_count; i < requested_channel_count; ++i)
        {
            ImDrawChannel& channel = splitter._Channels[i];

            // If we're inside the old capacity region of the array, we need to reuse the existing
            // memory of the command and index buffers.
            if (i < old_channel_capacity)
            {
                channel._CmdBuffer.resize(0);
                channel._IdxBuffer.resize(0);
            }
            // Else, we need to construct new draw channels.
            else
            {
                IM_PLACEMENT_NEW(&channel) ImDrawChannel();
            }

            {
                ImDrawCmd draw_cmd;
                draw_cmd.ClipRect = draw_list->_ClipRectStack.back();
                draw_cmd.TextureId = draw_list->_TextureIdStack.back();
                channel._CmdBuffer.push_back(draw_cmd);
            }
        }
    }

    void ImDrawListSplitterSwapChannels(
        ImDrawListSplitter& splitter,
        const int           lhs_idx,
        const int           rhs_idx)
    {
        if (lhs_idx == rhs_idx)
        {
            return;
        }

        assert(lhs_idx >= 0 && lhs_idx < splitter._Count);
        assert(rhs_idx >= 0 && rhs_idx < splitter._Count);

        ImDrawChannel& lhs_channel = splitter._Channels[lhs_idx];
        ImDrawChannel& rhs_channel = splitter._Channels[rhs_idx];
        lhs_channel._CmdBuffer.swap(rhs_channel._CmdBuffer);
        lhs_channel._IdxBuffer.swap(rhs_channel._IdxBuffer);

        const int current_channel = splitter._Current;

        if (current_channel == lhs_idx)
        {
            splitter._Current = rhs_idx;
        }
        else if (current_channel == rhs_idx)
        {
            splitter._Current = lhs_idx;
        }
    }

    void DrawListSet(ImDrawList* window_draw_list)
    {
        GImNodes.CanvasDrawList = window_draw_list;
        GImNodes.NodeIdxToSubmissionIdx.Clear();
        GImNodes.NodeIdxSubmissionOrder.clear();
    }

    // The draw list channels are structured as follows. First we have our base channel, the canvas grid
    // on which we render the grid lines in BeginNodeEditor(). The base channel is the reason
    // draw_list_submission_idx_to_background_channel_idx offsets the index by one. Each BeginNode()
    // call appends two new draw channels, for the node background and foreground. The node foreground
    // is the channel into which the node's ImGui content is rendered. Finally, in EndNodeEditor() we
    // append one last draw channel for rendering the selection box and the incomplete link on top of
    // everything else.
    //
    // +----------+----------+----------+----------+----------+----------+
    // |          |          |          |          |          |          |
    // |canvas    |node      |node      |...       |...       |click     |
    // |grid      |background|foreground|          |          |interaction
    // |          |          |          |          |          |          |
    // +----------+----------+----------+----------+----------+----------+
    //            |                     |
    //            |   submission idx    |
    //            |                     |
    //            -----------------------

    void DrawListAddNode(const int node_idx)
    {
        GImNodes.NodeIdxToSubmissionIdx.SetInt(
            static_cast<ImGuiID>(node_idx), GImNodes.NodeIdxSubmissionOrder.Size);
        GImNodes.NodeIdxSubmissionOrder.push_back(node_idx);
        ImDrawListGrowChannels(GImNodes.CanvasDrawList, 2);
    }

    void DrawListAppendClickInteractionChannel()
    {
        // NOTE: don't use this function outside of EndNodeEditor. Using this before all nodes have been
        // added will screw up the node draw order.
        ImDrawListGrowChannels(GImNodes.CanvasDrawList, 1);
    }

    int DrawListSubmissionIdxToBackgroundChannelIdx(const int submission_idx)
    {
        // NOTE: the first channel is the canvas background, i.e. the grid
        return 1 + 2 * submission_idx;
    }

    int DrawListSubmissionIdxToForegroundChannelIdx(const int submission_idx)
    {
        return DrawListSubmissionIdxToBackgroundChannelIdx(submission_idx) + 1;
    }

    void DrawListActivateClickInteractionChannel()
    {
        GImNodes.CanvasDrawList->_Splitter.SetCurrentChannel(
            GImNodes.CanvasDrawList, GImNodes.CanvasDrawList->_Splitter._Count - 1);
    }

    void DrawListActivateCurrentNodeForeground()
    {
        const int foreground_channel_idx =
            DrawListSubmissionIdxToForegroundChannelIdx(GImNodes.NodeIdxSubmissionOrder.Size - 1);
        GImNodes.CanvasDrawList->_Splitter.SetCurrentChannel(
            GImNodes.CanvasDrawList, foreground_channel_idx);
    }

    void DrawListActivateNodeBackground(const int node_idx)
    {
        const int submission_idx = GImNodes.NodeIdxToSubmissionIdx.GetInt(static_cast<ImGuiID>(node_idx), -1);
        // There is a discrepancy in the submitted node count and the rendered node count! Did you call
        // one of the following functions
        // * EditorContextMoveToNode
        // * SetNodeScreenSpacePos
        // * SetNodeGridSpacePos
        // * SetNodeDraggable
        // after the BeginNode/EndNode function calls?
//std::cout << "node_editor_basis_class :: DrawListActivateNodeBackground  000000 : " << submission_idx << std::endl;
        assert(submission_idx != -1);
//std::cout << "node_editor_basis_class :: DrawListActivateNodeBackground  11111 : " << submission_idx << std::endl;
        const int background_channel_idx = DrawListSubmissionIdxToBackgroundChannelIdx(submission_idx);
//std::cout << "node_editor_basis_class :: DrawListActivateNodeBackground  22222 : " << background_channel_idx << std::endl;
        GImNodes.CanvasDrawList->_Splitter.SetCurrentChannel(GImNodes.CanvasDrawList, background_channel_idx);
    }

    void DrawListSwapSubmissionIndices(const int lhs_idx, const int rhs_idx)
    {
        assert(lhs_idx != rhs_idx);

        const int lhs_foreground_channel_idx = DrawListSubmissionIdxToForegroundChannelIdx(lhs_idx);
        const int lhs_background_channel_idx = DrawListSubmissionIdxToBackgroundChannelIdx(lhs_idx);
        const int rhs_foreground_channel_idx = DrawListSubmissionIdxToForegroundChannelIdx(rhs_idx);
        const int rhs_background_channel_idx = DrawListSubmissionIdxToBackgroundChannelIdx(rhs_idx);

        ImDrawListSplitterSwapChannels(
            GImNodes.CanvasDrawList->_Splitter,
            lhs_background_channel_idx,
            rhs_background_channel_idx);
        ImDrawListSplitterSwapChannels(
            GImNodes.CanvasDrawList->_Splitter,
            lhs_foreground_channel_idx,
            rhs_foreground_channel_idx);
    }

    void DrawListSortChannelsByDepth(const ImVector<int>& node_idx_depth_order)
    {
        if (GImNodes.NodeIdxToSubmissionIdx.Data.Size < 2)
        {
            return;
        }

        assert(node_idx_depth_order.Size == GImNodes.NodeIdxSubmissionOrder.Size);

        int start_idx = node_idx_depth_order.Size - 1;

        while (node_idx_depth_order[start_idx] == GImNodes.NodeIdxSubmissionOrder[start_idx])
        {
            if (--start_idx == 0)
            {
                // early out if submission order and depth order are the same
                return;
            }
        }

        // TODO: this is an O(N^2) algorithm. It might be worthwhile revisiting this to see if the time
        // complexity can be reduced.

        for (int depth_idx = start_idx; depth_idx > 0; --depth_idx)
        {
            const int node_idx = node_idx_depth_order[depth_idx];

            // Find the current index of the node_idx in the submission order array
            int submission_idx = -1;
            for (int i = 0; i < GImNodes.NodeIdxSubmissionOrder.Size; ++i)
            {
                if (GImNodes.NodeIdxSubmissionOrder[i] == node_idx)
                {
                    submission_idx = i;
                    break;
                }
            }
            assert(submission_idx >= 0);

            if (submission_idx == depth_idx)
            {
                continue;
            }

            for (int j = submission_idx; j < depth_idx; ++j)
            {
                DrawListSwapSubmissionIndices(j, j + 1);
                ImSwap(GImNodes.NodeIdxSubmissionOrder[j], GImNodes.NodeIdxSubmissionOrder[j + 1]);
            }
        }
    }
    // [SECTION] ui state logic
public:
    ImVec2 GetScreenSpacePinCoordinates(
        const ImRect& node_rect,
        const ImRect& attribute_rect,
        const ImNodesAttributeType type)
    {
        assert(type == ImNodesAttributeType_Input || type == ImNodesAttributeType_Output);
        const float x = type == ImNodesAttributeType_Input
            ? (node_rect.Min.x - GImNodes.Style.PinOffset)
            : (node_rect.Max.x + GImNodes.Style.PinOffset);
        return ImVec2(x, 0.5f * (attribute_rect.Min.y + attribute_rect.Max.y));
    }

    ImVec2 GetScreenSpacePinCoordinates(ImNodesEditorContext *editor, const ImPinData& pin)
    {
        const ImRect& parent_node_rect = editor->Nodes.Pool[pin.ParentNodeIdx].Rect;
        return GetScreenSpacePinCoordinates(parent_node_rect, pin.AttributeRect, pin.Type);
    }

    bool MouseInCanvas()
    {
        // This flag should be true either when hovering or clicking something in the canvas.
        const bool is_window_hovered_or_focused = ImGui::IsWindowHovered() || ImGui::IsWindowFocused();

        return is_window_hovered_or_focused &&
            GImNodes.CanvasRectScreenSpace.Contains(ImGui::GetMousePos());
    }

protected:
    void BeginNodeSelection(ImNodesEditorContext *editor, const int node_idx)
    {
        // Don't start selecting a node if we are e.g. already creating and dragging
        // a new link! New link creation can happen when the mouse is clicked over
        // a node, but within the hover radius of a pin.
        if (editor->ClickInteraction.Type != ImNodesClickInteractionType_None)
        {
            return;
        }

        editor->ClickInteraction.Type = ImNodesClickInteractionType_Node;
        // If the node is not already contained in the selection, then we want only
        // the interaction node to be selected, effective immediately.
        //
        // Otherwise, we want to allow for the possibility of multiple nodes to be
        // moved at once.
        if (!editor->SelectedNodeIndices.contains(node_idx))
        {
            editor->SelectedNodeIndices.clear();
            editor->SelectedLinkIndices.clear();
            editor->SelectedNodeIndices.push_back(node_idx);

            // Ensure that individually selected nodes get rendered on top
            ImVector<int>& depth_stack = editor->NodeDepthOrder;
            const int* const elem = depth_stack.find(node_idx);
            assert(elem != depth_stack.end());
            depth_stack.erase(elem);
            depth_stack.push_back(node_idx);
        }
    }

    void BeginLinkSelection(ImNodesEditorContext *editor, const int link_idx)
    {
        editor->ClickInteraction.Type = ImNodesClickInteractionType_Link;
        // When a link is selected, clear all other selections, and insert the link
        // as the sole selection.
        editor->SelectedNodeIndices.clear();
        editor->SelectedLinkIndices.clear();
        editor->SelectedLinkIndices.push_back(link_idx);
    }

    void BeginLinkDetach(ImNodesEditorContext *editor, const int link_idx, const int detach_pin_idx)
    {
        const ImLinkData& link = editor->Links.Pool[link_idx];
        ImClickInteractionState& state = editor->ClickInteraction;
        state.Type = ImNodesClickInteractionType_LinkCreation;
        state.LinkCreation.EndPinIdx.Reset();
        state.LinkCreation.StartPinIdx =
            detach_pin_idx == link.StartPinIdx ? link.EndPinIdx : link.StartPinIdx;
        GImNodes.DeletedLinkIdx = link_idx;
    }

    void BeginLinkCreation(ImNodesEditorContext *editor, const int hovered_pin_idx)
    {
        editor->ClickInteraction.Type = ImNodesClickInteractionType_LinkCreation;
        editor->ClickInteraction.LinkCreation.StartPinIdx = hovered_pin_idx;
        editor->ClickInteraction.LinkCreation.EndPinIdx.Reset();
        editor->ClickInteraction.LinkCreation.Type = ImNodesLinkCreationType_Standard;
        GImNodes.ImNodesUIState |= ImNodesUIState_LinkStarted;
    }

    void BeginLinkInteraction(
        ImNodesEditorContext *editor,
        const int             link_idx,
        const ImOptionalIndex pin_idx = ImOptionalIndex())
    {
        // Check if we are clicking the link with the modifier pressed.
        // This will in a link detach via clicking.

        const bool modifier_pressed = GImNodes.Io.LinkDetachWithModifierClick.Modifier == NULL
            ? false
            : *GImNodes.Io.LinkDetachWithModifierClick.Modifier;

        if (modifier_pressed)
        {
            const ImLinkData& link = editor->Links.Pool[link_idx];
            const ImPinData& start_pin = editor->Pins.Pool[link.StartPinIdx];
            const ImPinData& end_pin = editor->Pins.Pool[link.EndPinIdx];
            const ImVec2& mouse_pos = GImNodes.MousePos;
            const float       dist_to_start = ImLengthSqr(start_pin.Pos - mouse_pos);
            const float       dist_to_end = ImLengthSqr(end_pin.Pos - mouse_pos);
            const int closest_pin_idx = dist_to_start < dist_to_end ? link.StartPinIdx : link.EndPinIdx;

            editor->ClickInteraction.Type = ImNodesClickInteractionType_LinkCreation;
            BeginLinkDetach(editor, link_idx, closest_pin_idx);
            editor->ClickInteraction.LinkCreation.Type = ImNodesLinkCreationType_FromDetach;
        }
        else
        {
            if (pin_idx.HasValue())
            {
                const int hovered_pin_flags = editor->Pins.Pool[pin_idx.Value()].Flags;

                // Check the 'click and drag to detach' case.
                if (hovered_pin_flags & ImNodesAttributeFlags_EnableLinkDetachWithDragClick)
                {
                    BeginLinkDetach(editor, link_idx, pin_idx.Value());
                    editor->ClickInteraction.LinkCreation.Type = ImNodesLinkCreationType_FromDetach;
                }
                else
                {
                    BeginLinkCreation(editor, pin_idx.Value());
                }
            }
            else
            {
                BeginLinkSelection(editor, link_idx);
            }
        }
    }

    //static inline bool IsMiniMapHovered();
    //bool IsMiniMapHovered();

    void BeginCanvasInteraction(ImNodesEditorContext *editor)
    {
        const bool any_ui_element_hovered =
            GImNodes.HoveredNodeIdx.HasValue() || GImNodes.HoveredLinkIdx.HasValue() ||
            GImNodes.HoveredPinIdx.HasValue() || ImGui::IsAnyItemHovered();

        const bool mouse_not_in_canvas = !MouseInCanvas();

        if (editor->ClickInteraction.Type != ImNodesClickInteractionType_None ||
            any_ui_element_hovered || mouse_not_in_canvas)
        {
            return;
        }

        const bool started_panning = GImNodes.AltMouseClicked;

        if (started_panning)
        {
            editor->ClickInteraction.Type = ImNodesClickInteractionType_Panning;
        }
        else if (GImNodes.LeftMouseClicked)
        {
            editor->ClickInteraction.Type = ImNodesClickInteractionType_BoxSelection;
            editor->ClickInteraction.BoxSelector.Rect.Min =
                ScreenSpaceToGridSpace(GImNodes.MousePos);
        }
    }

    void BoxSelectorUpdateSelection(ImNodesEditorContext *editor, ImRect box_rect)
    {
        // Invert box selector coordinates as needed

        if (box_rect.Min.x > box_rect.Max.x)
        {
            ImSwap(box_rect.Min.x, box_rect.Max.x);
        }

        if (box_rect.Min.y > box_rect.Max.y)
        {
            ImSwap(box_rect.Min.y, box_rect.Max.y);
        }

        // Update node selection

        editor->SelectedNodeIndices.clear();

        // Test for overlap against node rectangles

        for (int node_idx = 0; node_idx < editor->Nodes.Pool.size(); ++node_idx)
        {
            if (editor->Nodes.InUse[node_idx])
            {
                ImNodeData& node = editor->Nodes.Pool[node_idx];
                if (box_rect.Overlaps(node.Rect))
                {
                    editor->SelectedNodeIndices.push_back(node_idx);
                }
            }
        }

        // Update link selection

        editor->SelectedLinkIndices.clear();

        // Test for overlap against links

        for (int link_idx = 0; link_idx < editor->Links.Pool.size(); ++link_idx)
        {
            if (editor->Links.InUse[link_idx])
            {
                const ImLinkData& link = editor->Links.Pool[link_idx];

                const ImPinData& pin_start = editor->Pins.Pool[link.StartPinIdx];
                const ImPinData& pin_end = editor->Pins.Pool[link.EndPinIdx];
                const ImRect& node_start_rect = editor->Nodes.Pool[pin_start.ParentNodeIdx].Rect;
                const ImRect& node_end_rect = editor->Nodes.Pool[pin_end.ParentNodeIdx].Rect;

                const ImVec2 start = GetScreenSpacePinCoordinates(
                    node_start_rect, pin_start.AttributeRect, pin_start.Type);
                const ImVec2 end =
                    GetScreenSpacePinCoordinates(node_end_rect, pin_end.AttributeRect, pin_end.Type);

                // Test
                if (RectangleOverlapsLink(box_rect, start, end, pin_start.Type))
                {
                    editor->SelectedLinkIndices.push_back(link_idx);
                }
            }
        }
    }

    void TranslateSelectedNodes(ImNodesEditorContext *editor)
    {
        if (GImNodes.LeftMouseDragging)
        {
            const ImGuiIO& io = ImGui::GetIO();
            for (int i = 0; i < editor->SelectedNodeIndices.size(); ++i)
            {
                const int   node_idx = editor->SelectedNodeIndices[i];
                ImNodeData& node = editor->Nodes.Pool[node_idx];
                if (node.Draggable)
                {
                    node.Origin += io.MouseDelta - editor->AutoPanningDelta;
                }
            }
        }
    }

    struct LinkPredicate
    {
        bool operator()(const ImLinkData& lhs, const ImLinkData& rhs) const
        {
            // Do a unique compare by sorting the pins' addresses.
            // This catches duplicate links, whether they are in the
            // same direction or not.
            // Sorting by pin index should have the uniqueness guarantees as sorting
            // by id -- each unique id will get one slot in the link pool array.

            int lhs_start = lhs.StartPinIdx;
            int lhs_end = lhs.EndPinIdx;
            int rhs_start = rhs.StartPinIdx;
            int rhs_end = rhs.EndPinIdx;

            if (lhs_start > lhs_end)
            {
                ImSwap(lhs_start, lhs_end);
            }

            if (rhs_start > rhs_end)
            {
                ImSwap(rhs_start, rhs_end);
            }

            return lhs_start == rhs_start && lhs_end == rhs_end;
        }
    };

public:
    ImOptionalIndex FindDuplicateLink(
        const ImNodesEditorContext *editor,
        const int                   start_pin_idx,
        const int                   end_pin_idx)
    {
        ImLinkData test_link(0);
        test_link.StartPinIdx = start_pin_idx;
        test_link.EndPinIdx = end_pin_idx;
        for (int link_idx = 0; link_idx < editor->Links.Pool.size(); ++link_idx)
        {
            const ImLinkData& link = editor->Links.Pool[link_idx];
            if (LinkPredicate()(test_link, link) && editor->Links.InUse[link_idx])
            {
                return ImOptionalIndex(link_idx);
            }
        }

        return ImOptionalIndex();
    }

    bool ShouldLinkSnapToPin(
        const ImNodesEditorContext *editor,
        const ImPinData& start_pin,
        const int                   hovered_pin_idx,
        const ImOptionalIndex       duplicate_link)
    {
        const ImPinData& end_pin = editor->Pins.Pool[hovered_pin_idx];

        // The end pin must be in a different node
        if (start_pin.ParentNodeIdx == end_pin.ParentNodeIdx)
        {
            return false;
        }

        // The end pin must be of a different type
        if (start_pin.Type == end_pin.Type)
        {
            return false;
        }

        // The link to be created must not be a duplicate, unless it is the link which was created on
        // snap. In that case we want to snap, since we want it to appear visually as if the created
        // link remains snapped to the pin.
        if (duplicate_link.HasValue() && !(duplicate_link == GImNodes.SnapLinkIdx))
        {
            return false;
        }

        return true;
    }

    void ClickInteractionUpdate(ImNodesEditorContext *editor)
    {
        switch (editor->ClickInteraction.Type)
        {
        case ImNodesClickInteractionType_BoxSelection:
        {
            editor->ClickInteraction.BoxSelector.Rect.Max =
                ScreenSpaceToGridSpace(GImNodes.MousePos);

            ImRect box_rect = editor->ClickInteraction.BoxSelector.Rect;
            box_rect.Min = GridSpaceToScreenSpace(box_rect.Min);
            box_rect.Max = GridSpaceToScreenSpace(box_rect.Max);

            BoxSelectorUpdateSelection(editor, box_rect);

            const ImU32 box_selector_color = GImNodes.Style.Colors[ImNodesCol_BoxSelector];
            const ImU32 box_selector_outline = GImNodes.Style.Colors[ImNodesCol_BoxSelectorOutline];
            GImNodes.CanvasDrawList->AddRectFilled(box_rect.Min, box_rect.Max, box_selector_color);
            GImNodes.CanvasDrawList->AddRect(box_rect.Min, box_rect.Max, box_selector_outline);

            if (GImNodes.LeftMouseReleased)
            {
                ImVector<int>& depth_stack = editor->NodeDepthOrder;
                const ImVector<int>& selected_idxs = editor->SelectedNodeIndices;

                // Bump the selected node indices, in order, to the top of the depth stack.
                // NOTE: this algorithm has worst case time complexity of O(N^2), if the node selection
                // is ~ N (due to selected_idxs.contains()).

                if ((selected_idxs.Size > 0) && (selected_idxs.Size < depth_stack.Size))
                {
                    int num_moved = 0; // The number of indices moved. Stop after selected_idxs.Size
                    for (int i = 0; i < depth_stack.Size - selected_idxs.Size; ++i)
                    {
                        for (int node_idx = depth_stack[i]; selected_idxs.contains(node_idx);
                            node_idx = depth_stack[i])
                        {
                            depth_stack.erase(depth_stack.begin() + static_cast<size_t>(i));
                            depth_stack.push_back(node_idx);
                            ++num_moved;
                        }

                        if (num_moved == selected_idxs.Size)
                        {
                            break;
                        }
                    }
                }

                editor->ClickInteraction.Type = ImNodesClickInteractionType_None;
            }
        }
        break;
        case ImNodesClickInteractionType_Node:
        {
            TranslateSelectedNodes(editor);

            if (GImNodes.LeftMouseReleased)
            {
                editor->ClickInteraction.Type = ImNodesClickInteractionType_None;
            }
        }
        break;
        case ImNodesClickInteractionType_Link:
        {
            if (GImNodes.LeftMouseReleased)
            {
                editor->ClickInteraction.Type = ImNodesClickInteractionType_None;
            }
        }
        break;
        case ImNodesClickInteractionType_LinkCreation:
        {
            const ImPinData& start_pin =
                editor->Pins.Pool[editor->ClickInteraction.LinkCreation.StartPinIdx];

            const ImOptionalIndex maybe_duplicate_link_idx =
                GImNodes.HoveredPinIdx.HasValue()
                ? FindDuplicateLink(
                    editor,
                    editor->ClickInteraction.LinkCreation.StartPinIdx,
                    GImNodes.HoveredPinIdx.Value())
                : ImOptionalIndex();

            const bool should_snap =
                GImNodes.HoveredPinIdx.HasValue() &&
                ShouldLinkSnapToPin(
                    editor, start_pin, GImNodes.HoveredPinIdx.Value(), maybe_duplicate_link_idx);

            // If we created on snap and the hovered pin is empty or changed, then we need signal that
            // the link's state has changed.
            const bool snapping_pin_changed =
                editor->ClickInteraction.LinkCreation.EndPinIdx.HasValue() &&
                !(GImNodes.HoveredPinIdx == editor->ClickInteraction.LinkCreation.EndPinIdx);

            // Detach the link that was created by this link event if it's no longer in snap range
            if (snapping_pin_changed && GImNodes.SnapLinkIdx.HasValue())
            {
                BeginLinkDetach(
                    editor,
                    GImNodes.SnapLinkIdx.Value(),
                    editor->ClickInteraction.LinkCreation.EndPinIdx.Value());
            }

            const ImVec2 start_pos = GetScreenSpacePinCoordinates(editor, start_pin);
            // If we are within the hover radius of a receiving pin, snap the link
            // endpoint to it
            const ImVec2 end_pos = should_snap
                ? GetScreenSpacePinCoordinates(
                    editor, editor->Pins.Pool[GImNodes.HoveredPinIdx.Value()])
                : GImNodes.MousePos;

            const CubicBezier cubic_bezier = GetCubicBezier(
                start_pos, end_pos, start_pin.Type, GImNodes.Style.LinkLineSegmentsPerLength);
                #if IMGUI_VERSION_NUM < 18000
                    GImNodes.CanvasDrawList->AddBezierCurve(
                #else
                    GImNodes.CanvasDrawList->AddBezierCubic(
                #endif
                cubic_bezier.P0,
                cubic_bezier.P1,
                cubic_bezier.P2,
                cubic_bezier.P3,
                GImNodes.Style.Colors[ImNodesCol_Link],
                GImNodes.Style.LinkThickness,
                cubic_bezier.NumSegments);

            const bool link_creation_on_snap =
                GImNodes.HoveredPinIdx.HasValue() &&
                (editor->Pins.Pool[GImNodes.HoveredPinIdx.Value()].Flags &
                    ImNodesAttributeFlags_EnableLinkCreationOnSnap);

            if (!should_snap)
            {
                editor->ClickInteraction.LinkCreation.EndPinIdx.Reset();
            }

            const bool create_link =
                should_snap && (GImNodes.LeftMouseReleased || link_creation_on_snap);

            if (create_link && !maybe_duplicate_link_idx.HasValue())
            {
                // Avoid send OnLinkCreated() events every frame if the snap link is not saved
                // (only applies for EnableLinkCreationOnSnap)
                if (!GImNodes.LeftMouseReleased &&
                    editor->ClickInteraction.LinkCreation.EndPinIdx == GImNodes.HoveredPinIdx)
                {
                    break;
                }

                GImNodes.ImNodesUIState |= ImNodesUIState_LinkCreated;
                editor->ClickInteraction.LinkCreation.EndPinIdx = GImNodes.HoveredPinIdx.Value();
            }

            if (GImNodes.LeftMouseReleased)
            {
                editor->ClickInteraction.Type = ImNodesClickInteractionType_None;
                if (!create_link)
                {
                    GImNodes.ImNodesUIState |= ImNodesUIState_LinkDropped;
                }
            }
        }
        break;
        case ImNodesClickInteractionType_Panning:
        {
            const bool dragging = GImNodes.AltMouseDragging;

            if (dragging)
            {
                editor->Panning += ImGui::GetIO().MouseDelta;
            }
            else
            {
                editor->ClickInteraction.Type = ImNodesClickInteractionType_None;
            }
        }
        break;
        case ImNodesClickInteractionType_ImGuiItem:
        {
            if (GImNodes.LeftMouseReleased)
            {
                editor->ClickInteraction.Type = ImNodesClickInteractionType_None;
            }
        }
        case ImNodesClickInteractionType_None:
            break;
        default:
            assert(!"Unreachable code!");
            break;
        }
    }

private:
    void ResolveOccludedPins(const ImNodesEditorContext *editor, ImVector<int>& occluded_pin_indices)
    {
        const ImVector<int>& depth_stack = editor->NodeDepthOrder;

        occluded_pin_indices.resize(0);

        if (depth_stack.Size < 2)
        {
            return;
        }

        // For each node in the depth stack
        for (int depth_idx = 0; depth_idx < (depth_stack.Size - 1); ++depth_idx)
        {
            const ImNodeData& node_below = editor->Nodes.Pool[depth_stack[depth_idx]];

            // Iterate over the rest of the depth stack to find nodes overlapping the pins
            for (int next_depth_idx = depth_idx + 1; next_depth_idx < depth_stack.Size;
                ++next_depth_idx)
            {
                const ImRect& rect_above = editor->Nodes.Pool[depth_stack[next_depth_idx]].Rect;

                // Iterate over each pin
                for (int idx = 0; idx < node_below.PinIndices.Size; ++idx)
                {
                    const int     pin_idx = node_below.PinIndices[idx];
                    const ImVec2& pin_pos = editor->Pins.Pool[pin_idx].Pos;

                    if (rect_above.Contains(pin_pos))
                    {
                        occluded_pin_indices.push_back(pin_idx);
                    }
                }
            }
        }
    }

    ImOptionalIndex ResolveHoveredPin(
        const ImObjectPool<ImPinData>& pins,
        const ImVector<int>& occluded_pin_indices)
    {
        float           smallest_distance = FLT_MAX;
        ImOptionalIndex pin_idx_with_smallest_distance;

        const float hover_radius_sqr = GImNodes.Style.PinHoverRadius * GImNodes.Style.PinHoverRadius;

        for (int idx = 0; idx < pins.Pool.Size; ++idx)
        {
            if (!pins.InUse[idx])
            {
                continue;
            }

            if (occluded_pin_indices.contains(idx))
            {
                continue;
            }

            const ImVec2& pin_pos = pins.Pool[idx].Pos;
            const float   distance_sqr = ImLengthSqr(pin_pos - GImNodes.MousePos);

            // TODO: GImNodes.Style.PinHoverRadius needs to be copied into pin data and the pin-local
            // value used here. This is no longer called in BeginAttribute/EndAttribute scope and the
            // detected pin might have a different hover radius than what the user had when calling
            // BeginAttribute/EndAttribute.
            if (distance_sqr < hover_radius_sqr && distance_sqr < smallest_distance)
            {
                smallest_distance = distance_sqr;
                pin_idx_with_smallest_distance = idx;
            }
        }

        return pin_idx_with_smallest_distance;
    }

    ImOptionalIndex ResolveHoveredNode(const ImVector<int>& depth_stack)
    {
        if (GImNodes.NodeIndicesOverlappingWithMouse.size() == 0)
        {
            return ImOptionalIndex();
        }

        if (GImNodes.NodeIndicesOverlappingWithMouse.size() == 1)
        {
            return ImOptionalIndex(GImNodes.NodeIndicesOverlappingWithMouse[0]);
        }

        int largest_depth_idx = -1;
        int node_idx_on_top = -1;

        for (int i = 0; i < GImNodes.NodeIndicesOverlappingWithMouse.size(); ++i)
        {
            const int node_idx = GImNodes.NodeIndicesOverlappingWithMouse[i];
            for (int depth_idx = 0; depth_idx < depth_stack.size(); ++depth_idx)
            {
                if (depth_stack[depth_idx] == node_idx && (depth_idx > largest_depth_idx))
                {
                    largest_depth_idx = depth_idx;
                    node_idx_on_top = node_idx;
                }
            }
        }

        assert(node_idx_on_top != -1);
        return ImOptionalIndex(node_idx_on_top);
    }

    ImOptionalIndex ResolveHoveredLink(
        const ImObjectPool<ImLinkData>& links,
        const ImObjectPool<ImPinData>& pins)
    {
        float           smallest_distance = FLT_MAX;
        ImOptionalIndex link_idx_with_smallest_distance;

        // There are two ways a link can be detected as "hovered".
        // 1. The link is within hover distance to the mouse. The closest such link is selected as being
        // hovered over.
        // 2. If the link is connected to the currently hovered pin.
        //
        // The latter is a requirement for link detaching with drag click to work, as both a link and
        // pin are required to be hovered over for the feature to work.

        for (int idx = 0; idx < links.Pool.Size; ++idx)
        {
            if (!links.InUse[idx])
            {
                continue;
            }

            const ImLinkData& link = links.Pool[idx];
            const ImPinData& start_pin = pins.Pool[link.StartPinIdx];
            const ImPinData& end_pin = pins.Pool[link.EndPinIdx];

            // If there is a hovered pin links can only be considered hovered if they use that pin
            if (GImNodes.HoveredPinIdx.HasValue())
            {
                if (GImNodes.HoveredPinIdx == link.StartPinIdx ||
                    GImNodes.HoveredPinIdx == link.EndPinIdx)
                {
                    return idx;
                }
                continue;
            }

            // TODO: the calculated CubicBeziers could be cached since we generate them again when
            // rendering the links

            const CubicBezier cubic_bezier = GetCubicBezier(
                start_pin.Pos, end_pin.Pos, start_pin.Type, GImNodes.Style.LinkLineSegmentsPerLength);

            // The distance test
            {
                const ImRect link_rect = GetContainingRectForCubicBezier(cubic_bezier);

                // First, do a simple bounding box test against the box containing the link
                // to see whether calculating the distance to the link is worth doing.
                if (link_rect.Contains(GImNodes.MousePos))
                {
                    const float distance = GetDistanceToCubicBezier(
                        GImNodes.MousePos, cubic_bezier, cubic_bezier.NumSegments);

                    // TODO: GImNodes.Style.LinkHoverDistance could be also copied into ImLinkData,
                    // since we're not calling this function in the same scope as ImNodes::Link(). The
                    // rendered/detected link might have a different hover distance than what the user
                    // had specified when calling Link()
                    if (distance < GImNodes.Style.LinkHoverDistance && distance < smallest_distance)
                    {
                        smallest_distance = distance;
                        link_idx_with_smallest_distance = idx;
                    }
                }
            }
        }

        return link_idx_with_smallest_distance;
    }

    // [SECTION] render helpers
public:
    //inline ImRect GetItemRect() { return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()); }
    ImRect GetItemRect() { return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()); }

    //inline ImVec2 GetNodeTitleBarOrigin(const ImNodeData& node)
    ImVec2 GetNodeTitleBarOrigin(const ImNodeData& node)
    {
        return node.Origin + node.LayoutStyle.Padding;
    }

    //inline ImVec2 GetNodeContentOrigin(const ImNodeData& node)
    ImVec2 GetNodeContentOrigin(const ImNodeData& node)
    {
        const ImVec2 title_bar_height =
            ImVec2(0.f, node.TitleBarContentRect.GetHeight() + 2.0f * node.LayoutStyle.Padding.y);
        return node.Origin + title_bar_height + node.LayoutStyle.Padding;
    }

    //inline ImRect GetNodeTitleRect(const ImNodeData& node)
    ImRect GetNodeTitleRect(const ImNodeData& node)
    {
        ImRect expanded_title_rect = node.TitleBarContentRect;
        expanded_title_rect.Expand(node.LayoutStyle.Padding);

        return ImRect(
            expanded_title_rect.Min,
            expanded_title_rect.Min + ImVec2(node.Rect.GetWidth(), 0.f) +
            ImVec2(0.f, expanded_title_rect.GetHeight()));
    }
private:
    // Need to define a style major line thikness variable value to replace line_thickness
    // and a major grid spacing varioable to replace 5.0
    void DrawGrid(ImNodesEditorContext *editor, const ImVec2& canvas_size)
    {
        const ImVec2 offset = editor->Panning;

        float line_thickness;
        for (float x = fmodf(offset.x, GImNodes.Style.GridSpacing); x < canvas_size.x / editor->zoom_scale; x += GImNodes.Style.GridSpacing)
        {
            if (fmodf(x - offset.x, GImNodes.Style.GridSpacing * editor->grid_spacing_major) == 0)
                line_thickness = editor->grid_thickness_major * editor->zoom_scale;
            else
                line_thickness = editor->grid_thickness_minor * editor->zoom_scale;

            GImNodes.CanvasDrawList->AddLine(
                EditorSpaceToScreenSpace(ImVec2(x * editor->zoom_scale, 0.0f)),
                EditorSpaceToScreenSpace(ImVec2(x * editor->zoom_scale, canvas_size.y)),
                GImNodes.Style.Colors[ImNodesCol_GridLine], line_thickness);
        }

        for (float y = fmodf(offset.y, GImNodes.Style.GridSpacing); y < canvas_size.y / editor->zoom_scale; y += GImNodes.Style.GridSpacing)
        {
            if (fmodf(y - offset.y, GImNodes.Style.GridSpacing * editor->grid_spacing_major) == 0)
                line_thickness = editor->grid_thickness_major * editor->zoom_scale;
            else
                line_thickness = editor->grid_thickness_minor * editor->zoom_scale;

            GImNodes.CanvasDrawList->AddLine(
                EditorSpaceToScreenSpace(ImVec2(0.0f, y * editor->zoom_scale)),
                EditorSpaceToScreenSpace(ImVec2(canvas_size.x, y * editor->zoom_scale)),
                GImNodes.Style.Colors[ImNodesCol_GridLine], line_thickness);
        }
    }

    struct QuadOffsets
    {
        ImVec2 TopLeft, BottomLeft, BottomRight, TopRight;
    };

    QuadOffsets CalculateQuadOffsets(const float side_length)
    {
        const float half_side = 0.5f * side_length;

        QuadOffsets offset;

        offset.TopLeft = ImVec2(-half_side, half_side);
        offset.BottomLeft = ImVec2(-half_side, -half_side);
        offset.BottomRight = ImVec2(half_side, -half_side);
        offset.TopRight = ImVec2(half_side, half_side);

        return offset;
    }

    struct TriangleOffsets
    {
        ImVec2 TopLeft, BottomLeft, Right;
    };

    TriangleOffsets CalculateTriangleOffsets(const float side_length)
    {
        // Calculates the Vec2 offsets from an equilateral triangle's midpoint to
        // its vertices. Here is how the left_offset and right_offset are
        // calculated.
        //
        // For an equilateral triangle of side length s, the
        // triangle's height, h, is h = s * sqrt(3) / 2.
        //
        // The length from the base to the midpoint is (1 / 3) * h. The length from
        // the midpoint to the triangle vertex is (2 / 3) * h.
        const float sqrt_3 = sqrtf(3.0f);
        const float left_offset = -0.1666666666667f * sqrt_3 * side_length;
        const float right_offset = 0.333333333333f * sqrt_3 * side_length;
        const float vertical_offset = 0.5f * side_length;

        TriangleOffsets offset;
        offset.TopLeft = ImVec2(left_offset, vertical_offset);
        offset.BottomLeft = ImVec2(left_offset, -vertical_offset);
        offset.Right = ImVec2(right_offset, 0.f);

        return offset;
    }

    void DrawPinShape(const ImVec2& pin_pos, const ImPinData& pin, const ImU32 pin_color)
    {
        static const int CIRCLE_NUM_SEGMENTS = 8;

        switch (pin.Shape)
        {
        case ImNodesPinShape_Circle:
        {
            GImNodes.CanvasDrawList->AddCircle(
                pin_pos,
                GImNodes.Style.PinCircleRadius,
                pin_color,
                CIRCLE_NUM_SEGMENTS,
                GImNodes.Style.PinLineThickness);
        }
        break;
        case ImNodesPinShape_CircleFilled:
        {
            GImNodes.CanvasDrawList->AddCircleFilled(
                pin_pos, GImNodes.Style.PinCircleRadius, pin_color, CIRCLE_NUM_SEGMENTS);
        }
        break;
        case ImNodesPinShape_Quad:
        {
            const QuadOffsets offset = CalculateQuadOffsets(GImNodes.Style.PinQuadSideLength);
            GImNodes.CanvasDrawList->AddQuad(
                pin_pos + offset.TopLeft,
                pin_pos + offset.BottomLeft,
                pin_pos + offset.BottomRight,
                pin_pos + offset.TopRight,
                pin_color,
                GImNodes.Style.PinLineThickness);
        }
        break;
        case ImNodesPinShape_QuadFilled:
        {
            const QuadOffsets offset = CalculateQuadOffsets(GImNodes.Style.PinQuadSideLength);
            GImNodes.CanvasDrawList->AddQuadFilled(
                pin_pos + offset.TopLeft,
                pin_pos + offset.BottomLeft,
                pin_pos + offset.BottomRight,
                pin_pos + offset.TopRight,
                pin_color);
        }
        break;
        case ImNodesPinShape_Triangle:
        {
            const TriangleOffsets offset =
                CalculateTriangleOffsets(GImNodes.Style.PinTriangleSideLength);
            GImNodes.CanvasDrawList->AddTriangle(
                pin_pos + offset.TopLeft,
                pin_pos + offset.BottomLeft,
                pin_pos + offset.Right,
                pin_color,
                // NOTE: for some weird reason, the line drawn by AddTriangle is
                // much thinner than the lines drawn by AddCircle or AddQuad.
                // Multiplying the line thickness by two seemed to solve the
                // problem at a few different thickness values.
                2.f * GImNodes.Style.PinLineThickness);
        }
        break;
        case ImNodesPinShape_TriangleFilled:
        {
            const TriangleOffsets offset =
                CalculateTriangleOffsets(GImNodes.Style.PinTriangleSideLength);
            GImNodes.CanvasDrawList->AddTriangleFilled(
                pin_pos + offset.TopLeft,
                pin_pos + offset.BottomLeft,
                pin_pos + offset.Right,
                pin_color);
        }
        break;
        default:
            assert(!"Invalid PinShape value!");
            break;
        }
    }

    void DrawPin(ImNodesEditorContext *editor, const int pin_idx)
    {
        ImPinData& pin = editor->Pins.Pool[pin_idx];
        const ImRect& parent_node_rect = editor->Nodes.Pool[pin.ParentNodeIdx].Rect;

        pin.Pos = GetScreenSpacePinCoordinates(parent_node_rect, pin.AttributeRect, pin.Type);

        ImU32 pin_color = pin.ColorStyle.Background;

        if (GImNodes.HoveredPinIdx == pin_idx)
        {
            pin_color = pin.ColorStyle.Hovered;
        }

        DrawPinShape(pin.Pos, pin, pin_color);
    }

    void DrawNode(ImNodesEditorContext *editor, const int node_idx)
    {
        const ImNodeData& node = editor->Nodes.Pool[node_idx];
        ImGui::SetCursorPos(node.Origin + editor->Panning);

        const bool node_hovered =
            GImNodes.HoveredNodeIdx == node_idx &&
            editor->ClickInteraction.Type != ImNodesClickInteractionType_BoxSelection;

        ImU32 node_background = node.ColorStyle.Background;
        ImU32 titlebar_background = node.ColorStyle.Titlebar;

        if (editor->SelectedNodeIndices.contains(node_idx))
        {
            node_background = node.ColorStyle.BackgroundSelected;
            titlebar_background = node.ColorStyle.TitlebarSelected;
        }
        else if (node_hovered)
        {
            node_background = node.ColorStyle.BackgroundHovered;
            titlebar_background = node.ColorStyle.TitlebarHovered;
        }

        {
            // node base
            GImNodes.CanvasDrawList->AddRectFilled(
                node.Rect.Min, node.Rect.Max, node_background, node.LayoutStyle.CornerRounding);

            // title bar:
            if (node.TitleBarContentRect.GetHeight() > 0.f)
            {
                ImRect title_bar_rect = GetNodeTitleRect(node);

#if IMGUI_VERSION_NUM < 18200
                GImNodes.CanvasDrawList->AddRectFilled(
                    title_bar_rect.Min,
                    title_bar_rect.Max,
                    titlebar_background,
                    node.LayoutStyle.CornerRounding,
                    ImDrawCornerFlags_Top);
#else
                GImNodes.CanvasDrawList->AddRectFilled(
                    title_bar_rect.Min,
                    title_bar_rect.Max,
                    titlebar_background,
                    node.LayoutStyle.CornerRounding,
                    ImDrawFlags_RoundCornersTop);

#endif
            }

            if ((GImNodes.Style.Flags & ImNodesStyleFlags_NodeOutline) != 0)
            {
#if IMGUI_VERSION_NUM < 18200
                GImNodes.CanvasDrawList->AddRect(
                    node.Rect.Min,
                    node.Rect.Max,
                    node.ColorStyle.Outline,
                    node.LayoutStyle.CornerRounding,
                    ImDrawCornerFlags_All,
                    node.LayoutStyle.BorderThickness);
#else
                GImNodes.CanvasDrawList->AddRect(
                    node.Rect.Min,
                    node.Rect.Max,
                    node.ColorStyle.Outline,
                    node.LayoutStyle.CornerRounding,
                    ImDrawFlags_RoundCornersAll,
                    node.LayoutStyle.BorderThickness);
#endif
            }
        }

        for (int i = 0; i < node.PinIndices.size(); ++i)
        {
            DrawPin(editor, node.PinIndices[i]);
        }

        if (node_hovered)
        {
            GImNodes.HoveredNodeIdx = node_idx;
        }
    }

    void DrawLink(ImNodesEditorContext *editor, const int link_idx)
    {
        const ImLinkData& link = editor->Links.Pool[link_idx];
        const ImPinData& start_pin = editor->Pins.Pool[link.StartPinIdx];
        const ImPinData& end_pin = editor->Pins.Pool[link.EndPinIdx];

        const CubicBezier cubic_bezier = GetCubicBezier(
            start_pin.Pos, end_pin.Pos, start_pin.Type, GImNodes.Style.LinkLineSegmentsPerLength);

        const bool link_hovered =
            GImNodes.HoveredLinkIdx == link_idx &&
            editor->ClickInteraction.Type != ImNodesClickInteractionType_BoxSelection;

        if (link_hovered)
        {
            GImNodes.HoveredLinkIdx = link_idx;
        }

        // It's possible for a link to be deleted in begin_link_interaction. A user
        // may detach a link, resulting in the link wire snapping to the mouse
        // position.
        //
        // In other words, skip rendering the link if it was deleted.
        if (GImNodes.DeletedLinkIdx == link_idx)
        {
            return;
        }

        ImU32 link_color = link.ColorStyle.Base;
        if (editor->SelectedLinkIndices.contains(link_idx))
        {
            link_color = link.ColorStyle.Selected;
        }
        else if (link_hovered)
        {
            link_color = link.ColorStyle.Hovered;
        }

#if IMGUI_VERSION_NUM < 18000
        GImNodes.CanvasDrawList->AddBezierCurve(
#else
        GImNodes.CanvasDrawList->AddBezierCubic(
#endif
            cubic_bezier.P0,
            cubic_bezier.P1,
            cubic_bezier.P2,
            cubic_bezier.P3,
            link_color,
            GImNodes.Style.LinkThickness,
            cubic_bezier.NumSegments);
    }
public:
    void BeginPinAttribute(
        const int                  id,
        const ImNodesAttributeType type,
        const ImNodesPinShape      shape,
        const int                  node_idx)
    {
        // Make sure to call BeginNode() before calling
        // BeginAttribute()
        assert(GImNodes.CurrentScope == ImNodesScope_Node);
        GImNodes.CurrentScope = ImNodesScope_Attribute;

        ImGui::BeginGroup();
        ImGui::PushID(id);

        ImNodesEditorContext *editor = GImNodes.EditorCtx;

        GImNodes.CurrentAttributeId = id;

        const int pin_idx = ObjectPoolFindOrCreateIndex(editor->Pins, id);
        GImNodes.CurrentPinIdx = pin_idx;
        ImPinData& pin = editor->Pins.Pool[pin_idx];
        pin.Id = id;
        pin.ParentNodeIdx = node_idx;
        pin.Type = type;
        pin.Shape = shape;
        pin.Flags = GImNodes.CurrentAttributeFlags;
        pin.ColorStyle.Background = GImNodes.Style.Colors[ImNodesCol_Pin];
        pin.ColorStyle.Hovered = GImNodes.Style.Colors[ImNodesCol_PinHovered];
    }

    void EndPinAttribute()
    {
        assert(GImNodes.CurrentScope == ImNodesScope_Attribute);
        GImNodes.CurrentScope = ImNodesScope_Node;

        ImGui::PopID();
        ImGui::EndGroup();

        if (ImGui::IsItemActive())
        {
            GImNodes.ActiveAttribute = true;
            GImNodes.ActiveAttributeId = GImNodes.CurrentAttributeId;
        }

        ImNodesEditorContext *editor = GImNodes.EditorCtx;
        ImPinData& pin = editor->Pins.Pool[GImNodes.CurrentPinIdx];
        ImNodeData& node = editor->Nodes.Pool[GImNodes.CurrentNodeIdx];
        pin.AttributeRect = GetItemRect();
        node.PinIndices.push_back(GImNodes.CurrentPinIdx);
    }

    // [SECTION] selection helpers

    template<typename T>
    void SelectObject(const ImObjectPool<T>& objects, ImVector<int>& selected_indices, const int id)
    {
        const int idx = ObjectPoolFind(objects, id);
        assert(idx >= 0);
        assert(selected_indices.find(idx) == selected_indices.end());
        selected_indices.push_back(idx);
    }

    template<typename T>
    void ClearObjectSelection(
        const ImObjectPool<T>& objects,
        ImVector<int>& selected_indices,
        const int              id)
    {
        const int idx = ObjectPoolFind(objects, id);
        assert(idx >= 0);
        assert(selected_indices.find(idx) != selected_indices.end());
        selected_indices.find_erase_unsorted(idx);
    }

    template<typename T>
    bool IsObjectSelected(const ImObjectPool<T>& objects, ImVector<int>& selected_indices, const int id)
    {
        const int idx = ObjectPoolFind(objects, id);
        return selected_indices.find(idx) != selected_indices.end();
    }

    // |||||||||||||| From imnodes_internal ||||||||||||||||||||

// [SECTION] ObjectPool implementation
private:
    template<typename T>
    //static inline int ObjectPoolFind(const ImObjectPool<T>& objects, const int id)
    int ObjectPoolFind(const ImObjectPool<T>& objects, const int id)
    {
        const int index = objects.IdMap.GetInt(static_cast<ImGuiID>(id), -1);
        return index;
    }

    template<typename T>
    //static inline void ObjectPoolUpdate(ImObjectPool<T>& objects)
    void ObjectPoolUpdate(ImObjectPool<T>& objects)
    {
        for (int i = 0; i < objects.InUse.size(); ++i)
        {
            const int id = objects.Pool[i].Id;

            if (!objects.InUse[i] && objects.IdMap.GetInt(id, -1) == i)
            {
                objects.IdMap.SetInt(id, -1);
                objects.FreeList.push_back(i);
                (objects.Pool.Data + i)->~T();
            }
        }
    }

    //template<>
    //inline void ObjectPoolUpdate(ImObjectPool<ImNodeData>& nodes)
    void ObjectPoolUpdate(ImObjectPool<ImNodeData>& nodes)
    {
        for (int i = 0; i < nodes.InUse.size(); ++i)
        {
            if (nodes.InUse[i])
            {
                nodes.Pool[i].PinIndices.clear();
            }
            else
            {
                const int id = nodes.Pool[i].Id;

                if (nodes.IdMap.GetInt(id, -1) == i)
                {
                    // Remove node idx form depth stack the first time we detect that this idx slot is
                    // unused
                    //ImVector<int>& depth_stack = EditorContextGet().NodeDepthOrder;
                    ImVector<int>& depth_stack = GImNodes.EditorCtx->NodeDepthOrder;
                    const int* const elem = depth_stack.find(i);
                    assert(elem != depth_stack.end());
                    depth_stack.erase(elem);

                    nodes.IdMap.SetInt(id, -1);
                    nodes.FreeList.push_back(i);
                    (nodes.Pool.Data + i)->~ImNodeData();
                }
            }
        }
    }

    template<typename T>
    //static inline void ObjectPoolReset(ImObjectPool<T>& objects)
    void ObjectPoolReset(ImObjectPool<T>& objects)
    {
        if (!objects.InUse.empty())
        {
            memset(objects.InUse.Data, 0, objects.InUse.size_in_bytes());
        }
    }

    template<typename T>
    //static inline int ObjectPoolFindOrCreateIndex(ImObjectPool<T>& objects, const int id)
    int ObjectPoolFindOrCreateIndex(ImObjectPool<T>& objects, const int id)
    {
        int index = objects.IdMap.GetInt(static_cast<ImGuiID>(id), -1);

        // Construct new object
        if (index == -1)
        {
            if (objects.FreeList.empty())
            {
                index = objects.Pool.size();
                IM_ASSERT(objects.Pool.size() == objects.InUse.size());
                const int new_size = objects.Pool.size() + 1;
                objects.Pool.resize(new_size);
                objects.InUse.resize(new_size);
            }
            else
            {
                index = objects.FreeList.back();
                objects.FreeList.pop_back();
            }
            IM_PLACEMENT_NEW(objects.Pool.Data + index) T(id);
            objects.IdMap.SetInt(static_cast<ImGuiID>(id), index);
        }

        // Flag it as used
        objects.InUse[index] = true;

        return index;
    }

    //template<>
    //inline int ObjectPoolFindOrCreateIndex(ImObjectPool<ImNodeData>& nodes, const int node_id)
    int ObjectPoolFindOrCreateIndex(ImObjectPool<ImNodeData>& nodes, const int node_id)
    {
        int node_idx = nodes.IdMap.GetInt(static_cast<ImGuiID>(node_id), -1);

        // Construct new node
        if (node_idx == -1)
        {
            if (nodes.FreeList.empty())
            {
                node_idx = nodes.Pool.size();
                IM_ASSERT(nodes.Pool.size() == nodes.InUse.size());
                const int new_size = nodes.Pool.size() + 1;
                nodes.Pool.resize(new_size);
                nodes.InUse.resize(new_size);
            }
            else
            {
                node_idx = nodes.FreeList.back();
                nodes.FreeList.pop_back();
            }
            IM_PLACEMENT_NEW(nodes.Pool.Data + node_idx) ImNodeData(node_id);
            nodes.IdMap.SetInt(static_cast<ImGuiID>(node_id), node_idx);

            //ImNodesEditorContext& editor = EditorContextGet();
            ImNodesEditorContext *editor = GImNodes.EditorCtx;
            editor->NodeDepthOrder.push_back(node_idx);
        }

        // Flag node as used
        nodes.InUse[node_idx] = true;

        return node_idx;
    }

    template<typename T>
    //static inline T& ObjectPoolFindOrCreateObject(ImObjectPool<T>& objects, const int id)
    T& ObjectPoolFindOrCreateObject(ImObjectPool<T>& objects, const int id)
    {
        const int index = ObjectPoolFindOrCreateIndex(objects, id);
        return objects.Pool[index];
    }

    // |||||||||||||| Styles |||||||||||||||| 

    void StyleColorsDark()
    {
        GImNodes.Style.Colors[ImNodesCol_NodeBackground] = IM_COL32(50, 50, 50, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeBackgroundHovered] = IM_COL32(75, 75, 75, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32(75, 75, 75, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeOutline] = IM_COL32(100, 100, 100, 255);
        // title bar colors match ImGui's titlebg colors
        GImNodes.Style.Colors[ImNodesCol_TitleBar] = IM_COL32(41, 74, 122, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(66, 150, 250, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(66, 150, 250, 255);
        // link colors match ImGui's slider grab colors
        GImNodes.Style.Colors[ImNodesCol_Link] = IM_COL32(61, 133, 224, 200);
        GImNodes.Style.Colors[ImNodesCol_LinkHovered] = IM_COL32(66, 150, 250, 255);
        GImNodes.Style.Colors[ImNodesCol_LinkSelected] = IM_COL32(66, 150, 250, 255);
        // pin colors match ImGui's button colors
        GImNodes.Style.Colors[ImNodesCol_Pin] = IM_COL32(53, 150, 250, 180);
        GImNodes.Style.Colors[ImNodesCol_PinHovered] = IM_COL32(53, 150, 250, 255);

        GImNodes.Style.Colors[ImNodesCol_BoxSelector] = IM_COL32(61, 133, 224, 30);
        GImNodes.Style.Colors[ImNodesCol_BoxSelectorOutline] = IM_COL32(61, 133, 224, 150);

        GImNodes.Style.Colors[ImNodesCol_GridBackground] = IM_COL32(40, 40, 50, 200);
        GImNodes.Style.Colors[ImNodesCol_GridLine] = IM_COL32(200, 200, 200, 40);

        // minimap colors
        //GImNodes.Style.Colors[ImNodesCol_MiniMapBackground] = IM_COL32(25, 25, 25, 150);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapBackgroundHovered] = IM_COL32(25, 25, 25, 200);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapOutline] = IM_COL32(150, 150, 150, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapOutlineHovered] = IM_COL32(150, 150, 150, 200);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackground] = IM_COL32(200, 200, 200, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundHovered] = IM_COL32(200, 200, 200, 255);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundSelected] =
        //    GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundHovered];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeOutline] = IM_COL32(200, 200, 200, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapLink] = GImNodes.Style.Colors[ImNodesCol_Link];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapLinkSelected] =
        //    GImNodes.Style.Colors[ImNodesCol_LinkSelected];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapCanvas] = IM_COL32(200, 200, 200, 25);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapCanvasOutline] = IM_COL32(200, 200, 200, 200);
    }

    void StyleColorsClassic()
    {
        GImNodes.Style.Colors[ImNodesCol_NodeBackground] = IM_COL32(50, 50, 50, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeBackgroundHovered] = IM_COL32(75, 75, 75, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32(75, 75, 75, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeOutline] = IM_COL32(100, 100, 100, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBar] = IM_COL32(69, 69, 138, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(82, 82, 161, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(82, 82, 161, 255);
        GImNodes.Style.Colors[ImNodesCol_Link] = IM_COL32(255, 255, 255, 100);
        GImNodes.Style.Colors[ImNodesCol_LinkHovered] = IM_COL32(105, 99, 204, 153);
        GImNodes.Style.Colors[ImNodesCol_LinkSelected] = IM_COL32(105, 99, 204, 153);
        GImNodes.Style.Colors[ImNodesCol_Pin] = IM_COL32(89, 102, 156, 170);
        GImNodes.Style.Colors[ImNodesCol_PinHovered] = IM_COL32(102, 122, 179, 200);
        GImNodes.Style.Colors[ImNodesCol_BoxSelector] = IM_COL32(82, 82, 161, 100);
        GImNodes.Style.Colors[ImNodesCol_BoxSelectorOutline] = IM_COL32(82, 82, 161, 255);
        GImNodes.Style.Colors[ImNodesCol_GridBackground] = IM_COL32(40, 40, 50, 200);
        GImNodes.Style.Colors[ImNodesCol_GridLine] = IM_COL32(200, 200, 200, 40);

        // minimap colors
        //GImNodes.Style.Colors[ImNodesCol_MiniMapBackground] = IM_COL32(25, 25, 25, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapBackgroundHovered] = IM_COL32(25, 25, 25, 200);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapOutline] = IM_COL32(150, 150, 150, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapOutlineHovered] = IM_COL32(150, 150, 150, 200);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackground] = IM_COL32(200, 200, 200, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundSelected] =
        //    GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundHovered];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundSelected] = IM_COL32(200, 200, 240, 255);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeOutline] = IM_COL32(200, 200, 200, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapLink] = GImNodes.Style.Colors[ImNodesCol_Link];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapLinkSelected] =
        //    GImNodes.Style.Colors[ImNodesCol_LinkSelected];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapCanvas] = IM_COL32(200, 200, 200, 25);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapCanvasOutline] = IM_COL32(200, 200, 200, 200);
    }

    void StyleColorsLight()
    {
        GImNodes.Style.Colors[ImNodesCol_NodeBackground] = IM_COL32(240, 240, 240, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeBackgroundHovered] = IM_COL32(240, 240, 240, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeBackgroundSelected] = IM_COL32(240, 240, 240, 255);
        GImNodes.Style.Colors[ImNodesCol_NodeOutline] = IM_COL32(100, 100, 100, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBar] = IM_COL32(248, 248, 248, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(209, 209, 209, 255);
        GImNodes.Style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(209, 209, 209, 255);
        // original imgui values: 66, 150, 250
        GImNodes.Style.Colors[ImNodesCol_Link] = IM_COL32(66, 150, 250, 100);
        // original imgui values: 117, 138, 204
        GImNodes.Style.Colors[ImNodesCol_LinkHovered] = IM_COL32(66, 150, 250, 242);
        GImNodes.Style.Colors[ImNodesCol_LinkSelected] = IM_COL32(66, 150, 250, 242);
        // original imgui values: 66, 150, 250
        GImNodes.Style.Colors[ImNodesCol_Pin] = IM_COL32(66, 150, 250, 160);
        GImNodes.Style.Colors[ImNodesCol_PinHovered] = IM_COL32(66, 150, 250, 255);
        GImNodes.Style.Colors[ImNodesCol_BoxSelector] = IM_COL32(90, 170, 250, 30);
        GImNodes.Style.Colors[ImNodesCol_BoxSelectorOutline] = IM_COL32(90, 170, 250, 150);
        GImNodes.Style.Colors[ImNodesCol_GridBackground] = IM_COL32(225, 225, 225, 255);
        GImNodes.Style.Colors[ImNodesCol_GridLine] = IM_COL32(180, 180, 180, 100);

        // minimap colors
        //GImNodes.Style.Colors[ImNodesCol_MiniMapBackground] = IM_COL32(25, 25, 25, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapBackgroundHovered] = IM_COL32(25, 25, 25, 200);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapOutline] = IM_COL32(150, 150, 150, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapOutlineHovered] = IM_COL32(150, 150, 150, 200);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackground] = IM_COL32(200, 200, 200, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundSelected] =
        //    GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundHovered];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeBackgroundSelected] = IM_COL32(200, 200, 240, 255);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapNodeOutline] = IM_COL32(200, 200, 200, 100);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapLink] = GImNodes.Style.Colors[ImNodesCol_Link];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapLinkSelected] =
        //    GImNodes.Style.Colors[ImNodesCol_LinkSelected];
        //GImNodes.Style.Colors[ImNodesCol_MiniMapCanvas] = IM_COL32(200, 200, 200, 25);
        //GImNodes.Style.Colors[ImNodesCol_MiniMapCanvasOutline] = IM_COL32(200, 200, 200, 200);
    }

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
public:
    float       zoom_scale = 1.0f;
    float       location_zoom_scale = 1.0;
    bool        zoom_on = false;// +++++
protected:
    const float zoom_scale_min = 0.25f;
    const float zoom_scale_max = 2.0f;
    const float zoom_delta_scale = 0.05f;

    void update_canvas_zoom(bool restore = false) {
        //std::cout << "IN update_canvas_zoom " << zoom_scale << std::endl;
        if (restore) {
            location_zoom_scale = 1.0 / zoom_scale;
            zoom_scale = 1.0;
            return;
        }

        ImGuiIO io = ImGui::GetIO();
        if (io.MouseWheel != 0.0) {
            auto zoom = static_cast<int>(io.MouseWheel);
            if (zoom < 0) {
                if (zoom_scale_min > zoom_scale - zoom_delta_scale) {
                    zoom_scale = zoom_scale_min;
                    location_zoom_scale = 1.0;
                }
                else {
                    zoom_scale -= zoom_delta_scale;
                    location_zoom_scale = 1 - zoom_delta_scale;
                }
                zoom += 1;
            }

            if (zoom > 0) {
                if (zoom_scale_max < zoom_scale + zoom_delta_scale) {
                    zoom_scale = zoom_scale_max;
                    location_zoom_scale = 1.0;
                }
                else {
                    zoom_scale += zoom_delta_scale;
                    location_zoom_scale = 1 + zoom_delta_scale;
                }
                zoom -= 1;
            }
        }
    }
 public:
        // Move this function out of graph and into node editor !!!!!!!!! ??????
    void update_canvas_node_zoom(ImGuiWindow* window, ImNodesEditorContext& editor, ImNodeData node) {
        ImVec2                wsize = window->Size;
        ImVec2                offset = editor.Panning;

        ImVec2 ngp = node.Origin;

        //std::cout << "update_canvas_node_zoom:: 0000 " << ngp.x << " : " << ngp.y << " : " << node.Origin.x << " : " << node.Origin.y << " : " << editor.Panning.x << " : " << editor.Panning.y << " : " << std::endl;

        //std::cout << "update_canvas_node_zoom:: 11111 " << wsize.x << " : " << wsize.y << std::endl;
        ImVec2 midpoint = { wsize.x / 2,wsize.y / 2 };
        midpoint -= offset;// get editor coordinate of the window center point where zoom action ic centered
        // -= As scrolling left increases panning x location not decreasing it as expected
//std::cout << "update_canvas_node_zoom:: 22222 " << midpoint.x << " : " << midpoint.y << std::endl;
        ImVec2 rel_pos = node.Origin - midpoint; // relative local coordinate as if midpoint of window is the coordinate orgin (0,0)
        //std::cout << "update_canvas_node_zoom:: 3333 " << rel_pos.x << " : " << rel_pos.y << std::endl;

        ImVec2 new_rel_pos = rel_pos * location_zoom_scale; //scaled relative coordinate

        //std::cout << "update_canvas_node_zoom:: 4444 " << new_rel_pos.x << " : " << new_rel_pos.y << " : " << location_zoom_scale << std::endl;
        ImVec2 new_ngp = midpoint + new_rel_pos; // new node graph point location of scaled node
        //std::cout << "update_canvas_node_zoom:: 5555 " << new_ngp.x << " : " << new_ngp.y << std::endl;
        //ImNodes::SetNodeGridSpacePos(node.Id, new_ngp);
        SetNodeGridSpacePos(node.Id, new_ngp);

    }
    
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



    void BeginNode(const int node_id)
    {
        // Remember to call BeginNodeEditor before calling BeginNode
        assert(GImNodes.CurrentScope == ImNodesScope_Editor);
        GImNodes.CurrentScope = ImNodesScope_Node;

        ImNodesEditorContext *editor = EditorContextGet();

        const int node_idx = ObjectPoolFindOrCreateIndex(editor->Nodes, node_id);
        GImNodes.CurrentNodeIdx = node_idx;

        ImNodeData& node = editor->Nodes.Pool[node_idx];
        node.ColorStyle.Background = GImNodes.Style.Colors[ImNodesCol_NodeBackground];
        node.ColorStyle.BackgroundHovered = GImNodes.Style.Colors[ImNodesCol_NodeBackgroundHovered];
        node.ColorStyle.BackgroundSelected = GImNodes.Style.Colors[ImNodesCol_NodeBackgroundSelected];
        node.ColorStyle.Outline = GImNodes.Style.Colors[ImNodesCol_NodeOutline];
        node.ColorStyle.Titlebar = GImNodes.Style.Colors[ImNodesCol_TitleBar];
        node.ColorStyle.TitlebarHovered = GImNodes.Style.Colors[ImNodesCol_TitleBarHovered];
        node.ColorStyle.TitlebarSelected = GImNodes.Style.Colors[ImNodesCol_TitleBarSelected];
        node.LayoutStyle.CornerRounding = GImNodes.Style.NodeCornerRounding;
        node.LayoutStyle.Padding = GImNodes.Style.NodePadding;
        node.LayoutStyle.BorderThickness = GImNodes.Style.NodeBorderThickness;

        // ImGui::SetCursorPos sets the cursor position, local to the current widget
        // (in this case, the child object started in BeginNodeEditor). Use
        // ImGui::SetCursorScreenPos to set the screen space coordinates directly.
        //ImGui::SetCursorPos(GridSpaceToEditorSpace(*editor, GetNodeTitleBarOrigin(node)));
        ImGui::SetCursorPos(GridSpaceToEditorSpace(GetNodeTitleBarOrigin(node)));

        DrawListAddNode(node_idx);
        DrawListActivateCurrentNodeForeground();

        ImGui::PushID(node.Id);
        ImGui::BeginGroup();
    }

    void EndNode()
    {
        assert(GImNodes.CurrentScope == ImNodesScope_Node);
        GImNodes.CurrentScope = ImNodesScope_Editor;

        ImNodesEditorContext *editor = EditorContextGet();

        // The node's rectangle depends on the ImGui UI group size.
        ImGui::EndGroup();
        ImGui::PopID();

        ImNodeData& node = editor->Nodes.Pool[GImNodes.CurrentNodeIdx];
        node.Rect = GetItemRect();
        node.Rect.Expand(node.LayoutStyle.Padding);

        editor->GridContentBounds.Add(node.Origin);
        editor->GridContentBounds.Add(node.Origin + node.Rect.GetSize());

        if (node.Rect.Contains(GImNodes.MousePos))
        {
            GImNodes.NodeIndicesOverlappingWithMouse.push_back(GImNodes.CurrentNodeIdx);
        }
    }

    ImVec2 GetNodeDimensions(int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        const int             node_idx = ObjectPoolFind(editor->Nodes, node_id);
        assert(node_idx != -1); // invalid node_id
        const ImNodeData& node = editor->Nodes.Pool[node_idx];
        return node.Rect.GetSize();
    }

    void BeginNodeTitleBar()
    {
        assert(GImNodes.CurrentScope == ImNodesScope_Node);
        ImGui::BeginGroup();
    }

    void EndNodeTitleBar()
    {
        assert(GImNodes.CurrentScope == ImNodesScope_Node);
        ImGui::EndGroup();

        ImNodesEditorContext  *editor = EditorContextGet();
        ImNodeData& node = editor->Nodes.Pool[GImNodes.CurrentNodeIdx];
        node.TitleBarContentRect = GetItemRect();

        ImGui::ItemAdd(GetNodeTitleRect(node), ImGui::GetID("title_bar"));

        //ImGui::SetCursorPos(GridSpaceToEditorSpace(editor, GetNodeContentOrigin(node)));
        ImGui::SetCursorPos(GridSpaceToEditorSpace(GetNodeContentOrigin(node)));
    }

    void BeginInputAttribute(const int id, const ImNodesPinShape shape)
    {
        BeginPinAttribute(id, ImNodesAttributeType_Input, shape, GImNodes.CurrentNodeIdx);
    }

    void EndInputAttribute() { EndPinAttribute(); }

    void BeginOutputAttribute(const int id, const ImNodesPinShape shape)
    {
        BeginPinAttribute(id, ImNodesAttributeType_Output, shape, GImNodes.CurrentNodeIdx);
    }

    void EndOutputAttribute() { EndPinAttribute(); }

    void BeginStaticAttribute(const int id)
    {
        // Make sure to call BeginNode() before calling BeginAttribute()
        assert(GImNodes.CurrentScope == ImNodesScope_Node);
        GImNodes.CurrentScope = ImNodesScope_Attribute;

        GImNodes.CurrentAttributeId = id;

        ImGui::BeginGroup();
        ImGui::PushID(id);
    }

    void EndStaticAttribute()
    {
        // Make sure to call BeginNode() before calling BeginAttribute()
        assert(GImNodes.CurrentScope == ImNodesScope_Attribute);
        GImNodes.CurrentScope = ImNodesScope_Node;

        ImGui::PopID();
        ImGui::EndGroup();

        if (ImGui::IsItemActive())
        {
            GImNodes.ActiveAttribute = true;
            GImNodes.ActiveAttributeId = GImNodes.CurrentAttributeId;
        }
    }

    void PushAttributeFlag(const ImNodesAttributeFlags flag)
    {
        GImNodes.CurrentAttributeFlags |= flag;
        GImNodes.AttributeFlagStack.push_back(GImNodes.CurrentAttributeFlags);
    }

    void PopAttributeFlag()
    {
        // PopAttributeFlag called without a matching PushAttributeFlag!
        // The bottom value is always the default value, pushed in Initialize().
        assert(GImNodes.AttributeFlagStack.size() > 1);

        GImNodes.AttributeFlagStack.pop_back();
        GImNodes.CurrentAttributeFlags = GImNodes.AttributeFlagStack.back();
    }

    // ****************
    ImLinkData create_link(const int id, const int start_attr_id, const int end_attr_id)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_Editor);

        ImNodesEditorContext* editor = EditorContextGet();
        ImLinkData& link = ObjectPoolFindOrCreateObject(editor->Links, id);
        link.Id = id;
        link.StartPinIdx = ObjectPoolFindOrCreateIndex(editor->Pins, start_attr_id);
        link.EndPinIdx = ObjectPoolFindOrCreateIndex(editor->Pins, end_attr_id);
        link.ColorStyle.Base = GImNodes.Style.Colors[ImNodesCol_Link];
        link.ColorStyle.Hovered = GImNodes.Style.Colors[ImNodesCol_LinkHovered];
        link.ColorStyle.Selected = GImNodes.Style.Colors[ImNodesCol_LinkSelected];

        // Check if this link was created by the current link event
        //if ((editor->ClickInteraction.Type == ImNodesClickInteractionType_LinkCreation &&
        //    editor->Pins.Pool[link.EndPinIdx].Flags & ImNodesAttributeFlags_EnableLinkCreationOnSnap &&
        //    editor->ClickInteraction.LinkCreation.StartPinIdx == link.StartPinIdx &&
        //    editor->ClickInteraction.LinkCreation.EndPinIdx == link.EndPinIdx) ||
        //    (editor->ClickInteraction.LinkCreation.StartPinIdx == link.EndPinIdx &&
        //        editor->ClickInteraction.LinkCreation.EndPinIdx == link.StartPinIdx))
        //{
            //GImNodes.SnapLinkIdx = ObjectPoolFindOrCreateIndex(editor->Links, id);
        //}

        return link;
    }
    
    // ****************


    void Link(const int id, const int start_attr_id, const int end_attr_id)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_Editor);

        ImNodesEditorContext  *editor = EditorContextGet();
        ImLinkData& link = ObjectPoolFindOrCreateObject(editor->Links, id);
        link.Id = id;
        link.StartPinIdx = ObjectPoolFindOrCreateIndex(editor->Pins, start_attr_id);
        link.EndPinIdx = ObjectPoolFindOrCreateIndex(editor->Pins, end_attr_id);
        link.ColorStyle.Base = GImNodes.Style.Colors[ImNodesCol_Link];
        link.ColorStyle.Hovered = GImNodes.Style.Colors[ImNodesCol_LinkHovered];
        link.ColorStyle.Selected = GImNodes.Style.Colors[ImNodesCol_LinkSelected];

        // Check if this link was created by the current link event
        if ((editor->ClickInteraction.Type == ImNodesClickInteractionType_LinkCreation &&
            editor->Pins.Pool[link.EndPinIdx].Flags & ImNodesAttributeFlags_EnableLinkCreationOnSnap &&
            editor->ClickInteraction.LinkCreation.StartPinIdx == link.StartPinIdx &&
            editor->ClickInteraction.LinkCreation.EndPinIdx == link.EndPinIdx) ||
            (editor->ClickInteraction.LinkCreation.StartPinIdx == link.EndPinIdx &&
                editor->ClickInteraction.LinkCreation.EndPinIdx == link.StartPinIdx))
        {
            GImNodes.SnapLinkIdx = ObjectPoolFindOrCreateIndex(editor->Links, id);
        }
    }

    void PushColorStyle(const ImNodesCol item, unsigned int color)
    {
        GImNodes.ColorModifierStack.push_back(ImNodesColElement(GImNodes.Style.Colors[item], item));
        GImNodes.Style.Colors[item] = color;
    }

    void PopColorStyle()
    {
        assert(GImNodes.ColorModifierStack.size() > 0);
        const ImNodesColElement elem = GImNodes.ColorModifierStack.back();
        GImNodes.Style.Colors[elem.Item] = elem.Color;
        GImNodes.ColorModifierStack.pop_back();
    }
private:
    struct ImNodesStyleVarInfo
    {
        ImGuiDataType Type;
        ImU32         Count;
        ImU32         Offset;
        void* GetVarPtr(ImNodesStyle* style) const { return (void*)((unsigned char*)style + Offset); }
    };

    //static const ImNodesStyleVarInfo GStyleVarInfo[] = {
    ImNodesStyleVarInfo GStyleVarInfo[13] = {
        // ImNodesStyleVar_GridSpacing
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, GridSpacing)},
        // ImNodesStyleVar_NodeCornerRounding
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, NodeCornerRounding)},
        // ImNodesStyleVar_NodePadding
        {ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImNodesStyle, NodePadding)},
        // ImNodesStyleVar_NodeBorderThickness
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, NodeBorderThickness)},
        // ImNodesStyleVar_LinkThickness
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, LinkThickness)},
        // ImNodesStyleVar_LinkLineSegmentsPerLength
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, LinkLineSegmentsPerLength)},
        // ImNodesStyleVar_LinkHoverDistance
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, LinkHoverDistance)},
        // ImNodesStyleVar_PinCircleRadius
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, PinCircleRadius)},
        // ImNodesStyleVar_PinQuadSideLength
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, PinQuadSideLength)},
        // ImNodesStyleVar_PinTriangleSideLength
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, PinTriangleSideLength)},
        // ImNodesStyleVar_PinLineThickness
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, PinLineThickness)},
        // ImNodesStyleVar_PinHoverRadius
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, PinHoverRadius)},
        // ImNodesStyleVar_PinOffset
        {ImGuiDataType_Float, 1, (ImU32)IM_OFFSETOF(ImNodesStyle, PinOffset)},
        // ImNodesStyleVar_MiniMapPadding
        //{ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImNodesStyle, MiniMapPadding)},
        // ImNodesStyleVar_MiniMapOffset
        //{ImGuiDataType_Float, 2, (ImU32)IM_OFFSETOF(ImNodesStyle, MiniMapOffset)},
    };
public:
    //static const ImNodesStyleVarInfo* GetStyleVarInfo(ImNodesStyleVar idx)
    ImNodesStyleVarInfo* GetStyleVarInfo(ImNodesStyleVar idx)
    {
        IM_ASSERT(idx >= 0 && idx < ImNodesStyleVar_COUNT);
        IM_ASSERT(IM_ARRAYSIZE(GStyleVarInfo) == ImNodesStyleVar_COUNT);
        return &GStyleVarInfo[idx];
    }

    void PushStyleVar(const ImNodesStyleVar item, const float value)
    {
        const ImNodesStyleVarInfo* var_info = GetStyleVarInfo(item);
        if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
        {
            float& style_var = *(float*)var_info->GetVarPtr(&GImNodes.Style);
            GImNodes.StyleModifierStack.push_back(ImNodesStyleVarElement(item, style_var));
            style_var = value;
            return;
        }
        IM_ASSERT(0 && "Called PushStyleVar() float variant but variable is not a float!");
    }

    void PushStyleVar(const ImNodesStyleVar item, const ImVec2& value)
    {
        const ImNodesStyleVarInfo* var_info = GetStyleVarInfo(item);
        if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
        {
            ImVec2& style_var = *(ImVec2*)var_info->GetVarPtr(&GImNodes.Style);
            GImNodes.StyleModifierStack.push_back(ImNodesStyleVarElement(item, style_var));
            style_var = value;
            return;
        }
        IM_ASSERT(0 && "Called PushStyleVar() ImVec2 variant but variable is not a ImVec2!");
    }

    void PopStyleVar(int count)
    {
        while (count > 0)
        {
            assert(GImNodes.StyleModifierStack.size() > 0);
            const ImNodesStyleVarElement style_backup = GImNodes.StyleModifierStack.back();
            GImNodes.StyleModifierStack.pop_back();
            const ImNodesStyleVarInfo* var_info = GetStyleVarInfo(style_backup.Item);
            void* style_var = var_info->GetVarPtr(&GImNodes.Style);
            if (var_info->Type == ImGuiDataType_Float && var_info->Count == 1)
            {
                ((float*)style_var)[0] = style_backup.FloatValue[0];
            }
            else if (var_info->Type == ImGuiDataType_Float && var_info->Count == 2)
            {
                ((float*)style_var)[0] = style_backup.FloatValue[0];
                ((float*)style_var)[1] = style_backup.FloatValue[1];
            }
            count--;
        }
    }

    void SetNodeScreenSpacePos(const int node_id, const ImVec2& screen_space_pos)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        ImNodeData& node = ObjectPoolFindOrCreateObject(editor->Nodes, node_id);
        //node.Origin = ScreenSpaceToGridSpace(editor, screen_space_pos);
        node.Origin = ScreenSpaceToGridSpace(screen_space_pos);
    }

    void SetNodeEditorSpacePos(const int node_id, const ImVec2& editor_space_pos)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        ImNodeData& node = ObjectPoolFindOrCreateObject(editor->Nodes, node_id);
        //node.Origin = EditorSpaceToGridSpace(editor, editor_space_pos);
        node.Origin = EditorSpaceToGridSpace(editor_space_pos);
    }

    void SetNodeGridSpacePos(const int node_id, const ImVec2& grid_pos)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        ImNodeData& node = ObjectPoolFindOrCreateObject(editor->Nodes, node_id);
        node.Origin = grid_pos;
    }

    void SetNodeDraggable(const int node_id, const bool draggable)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        ImNodeData& node = ObjectPoolFindOrCreateObject(editor->Nodes, node_id);
        node.Draggable = draggable;
    }

    ImVec2 GetNodeScreenSpacePos(const int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        const int             node_idx = ObjectPoolFind(editor->Nodes, node_id);
        assert(node_idx != -1);
        ImNodeData& node = editor->Nodes.Pool[node_idx];
        //return GridSpaceToScreenSpace(editor, node.Origin);
        return GridSpaceToScreenSpace(node.Origin);
    }

    ImVec2 GetNodeEditorSpacePos(const int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        const int             node_idx = ObjectPoolFind(editor->Nodes, node_id);
        assert(node_idx != -1);
        ImNodeData& node = editor->Nodes.Pool[node_idx];
        //return GridSpaceToEditorSpace(editor, node.Origin);
        return GridSpaceToEditorSpace(node.Origin);
    }

    ImVec2 GetNodeGridSpacePos(const int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        const int             node_idx = ObjectPoolFind(editor->Nodes, node_id);
        assert(node_idx != -1);
        ImNodeData& node = editor->Nodes.Pool[node_idx];
        return node.Origin;
    }

    bool IsEditorHovered() { return MouseInCanvas(); }

    bool IsNodeHovered(int* const node_id)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        assert(node_id != NULL);

        const bool is_hovered = GImNodes.HoveredNodeIdx.HasValue();
        if (is_hovered)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            *node_id = editor->Nodes.Pool[GImNodes.HoveredNodeIdx.Value()].Id;
        }
        return is_hovered;
    }

    bool IsLinkHovered(int* const link_id)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        assert(link_id != NULL);

        const bool is_hovered = GImNodes.HoveredLinkIdx.HasValue();
        if (is_hovered)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            *link_id = editor->Links.Pool[GImNodes.HoveredLinkIdx.Value()].Id;
        }
        return is_hovered;
    }

    bool IsPinHovered(int* const attr)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        assert(attr != NULL);

        const bool is_hovered = GImNodes.HoveredPinIdx.HasValue();
        if (is_hovered)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            *attr = editor->Pins.Pool[GImNodes.HoveredPinIdx.Value()].Id;
        }
        return is_hovered;
    }

    int NumSelectedNodes()
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        const ImNodesEditorContext  *editor = EditorContextGet();
        return editor->SelectedNodeIndices.size();
    }

    int NumSelectedLinks()
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        const ImNodesEditorContext  *editor = EditorContextGet();
        return editor->SelectedLinkIndices.size();
    }

    void GetSelectedNodes(int* node_ids)
    {
        assert(node_ids != NULL);

        const ImNodesEditorContext  *editor = EditorContextGet();
        for (int i = 0; i < editor->SelectedNodeIndices.size(); ++i)
        {
            const int node_idx = editor->SelectedNodeIndices[i];
            node_ids[i] = editor->Nodes.Pool[node_idx].Id;
        }
    }

    void GetSelectedLinks(int* link_ids)
    {
        assert(link_ids != NULL);

        const ImNodesEditorContext  *editor = EditorContextGet();
        for (int i = 0; i < editor->SelectedLinkIndices.size(); ++i)
        {
            const int link_idx = editor->SelectedLinkIndices[i];
            link_ids[i] = editor->Links.Pool[link_idx].Id;
        }
    }

    void ClearNodeSelection()
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        editor->SelectedNodeIndices.clear();
    }

    void ClearNodeSelection(int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        ClearObjectSelection(editor->Nodes, editor->SelectedNodeIndices, node_id);
    }

    void ClearLinkSelection()
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        editor->SelectedLinkIndices.clear();
    }

    void ClearLinkSelection(int link_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        ClearObjectSelection(editor->Links, editor->SelectedLinkIndices, link_id);
    }

    void SelectNode(int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        SelectObject(editor->Nodes, editor->SelectedNodeIndices, node_id);
    }

    void SelectLink(int link_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        SelectObject(editor->Links, editor->SelectedLinkIndices, link_id);
    }

    bool IsNodeSelected(int node_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        return IsObjectSelected(editor->Nodes, editor->SelectedNodeIndices, node_id);
    }

    bool IsLinkSelected(int link_id)
    {
        ImNodesEditorContext  *editor = EditorContextGet();
        return IsObjectSelected(editor->Links, editor->SelectedLinkIndices, link_id);
    }

    bool IsAttributeActive()
    {
        assert((GImNodes.CurrentScope & ImNodesScope_Node) != 0);

        if (!GImNodes.ActiveAttribute)
        {
            return false;
        }

        return GImNodes.ActiveAttributeId == GImNodes.CurrentAttributeId;
    }

    bool IsAnyAttributeActive(int* const attribute_id)
    {
        assert((GImNodes.CurrentScope & (ImNodesScope_Node | ImNodesScope_Attribute)) == 0);

        if (!GImNodes.ActiveAttribute)
        {
            return false;
        }

        if (attribute_id != NULL)
        {
            *attribute_id = GImNodes.ActiveAttributeId;
        }

        return true;
    }

    bool IsLinkStarted(int* const started_at_id)
    {
        // Call this function after EndNodeEditor()!
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        assert(started_at_id != NULL);

        const bool is_started = (GImNodes.ImNodesUIState & ImNodesUIState_LinkStarted) != 0;
        if (is_started)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            const int                   pin_idx = editor->ClickInteraction.LinkCreation.StartPinIdx;
            *started_at_id = editor->Pins.Pool[pin_idx].Id;
        }

        return is_started;
    }

    bool IsLinkDropped(int* const started_at_id, const bool including_detached_links)
    {
        // Call this function after EndNodeEditor()!
        assert(GImNodes.CurrentScope == ImNodesScope_None);

        const ImNodesEditorContext  *editor = EditorContextGet();

        const bool link_dropped =
            (GImNodes.ImNodesUIState & ImNodesUIState_LinkDropped) != 0 &&
            (including_detached_links ||
                editor->ClickInteraction.LinkCreation.Type != ImNodesLinkCreationType_FromDetach);

        if (link_dropped && started_at_id)
        {
            const int pin_idx = editor->ClickInteraction.LinkCreation.StartPinIdx;
            *started_at_id = editor->Pins.Pool[pin_idx].Id;
        }

        return link_dropped;
    }

    bool IsLinkCreated(
        int* const  started_at_pin_id,
        int* const  ended_at_pin_id,
        bool* const created_from_snap = false)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        assert(started_at_pin_id != NULL);
        assert(ended_at_pin_id != NULL);

        const bool is_created = (GImNodes.ImNodesUIState & ImNodesUIState_LinkCreated) != 0;

        if (is_created)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            const int                   start_idx = editor->ClickInteraction.LinkCreation.StartPinIdx;
            const int        end_idx = editor->ClickInteraction.LinkCreation.EndPinIdx.Value();
            const ImPinData& start_pin = editor->Pins.Pool[start_idx];
            const ImPinData& end_pin = editor->Pins.Pool[end_idx];

            if (start_pin.Type == ImNodesAttributeType_Output)
            {
                *started_at_pin_id = start_pin.Id;
                *ended_at_pin_id = end_pin.Id;
            }
            else
            {
                *started_at_pin_id = end_pin.Id;
                *ended_at_pin_id = start_pin.Id;
            }

            if (created_from_snap)
            {
                *created_from_snap =
                    editor->ClickInteraction.Type == ImNodesClickInteractionType_LinkCreation;
            }
        }

        return is_created;
    }

    bool IsLinkCreated(
        int* started_at_node_id,
        int* started_at_pin_id,
        int* ended_at_node_id,
        int* ended_at_pin_id,
        bool* created_from_snap = false)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);
        assert(started_at_node_id != NULL);
        assert(started_at_pin_id != NULL);
        assert(ended_at_node_id != NULL);
        assert(ended_at_pin_id != NULL);

        const bool is_created = (GImNodes.ImNodesUIState & ImNodesUIState_LinkCreated) != 0;

        if (is_created)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            const int                   start_idx = editor->ClickInteraction.LinkCreation.StartPinIdx;
            const int         end_idx = editor->ClickInteraction.LinkCreation.EndPinIdx.Value();
            const ImPinData& start_pin = editor->Pins.Pool[start_idx];
            const ImNodeData& start_node = editor->Nodes.Pool[start_pin.ParentNodeIdx];
            const ImPinData& end_pin = editor->Pins.Pool[end_idx];
            const ImNodeData& end_node = editor->Nodes.Pool[end_pin.ParentNodeIdx];

            if (start_pin.Type == ImNodesAttributeType_Output)
            {
                *started_at_pin_id = start_pin.Id;
                *started_at_node_id = start_node.Id;
                *ended_at_pin_id = end_pin.Id;
                *ended_at_node_id = end_node.Id;
            }
            else
            {
                *started_at_pin_id = end_pin.Id;
                *started_at_node_id = end_node.Id;
                *ended_at_pin_id = start_pin.Id;
                *ended_at_node_id = start_node.Id;
            }

            if (created_from_snap)
            {
                *created_from_snap =
                    editor->ClickInteraction.Type == ImNodesClickInteractionType_LinkCreation;
            }
        }

        return is_created;
    }

    bool IsLinkDestroyed(int* const link_id)
    {
        assert(GImNodes.CurrentScope == ImNodesScope_None);

        const bool link_destroyed = GImNodes.DeletedLinkIdx.HasValue();
        if (link_destroyed)
        {
            const ImNodesEditorContext  *editor = EditorContextGet();
            const int                   link_idx = GImNodes.DeletedLinkIdx.Value();
            *link_id = editor->Links.Pool[link_idx].Id;
        }

        return link_destroyed;
    }

        void NodeLineHandler(ImNodesEditorContext  *editor, const char* const line)
        {
            int id;
            int x, y;
            if (sscanf(line, "[node.%i", &id) == 1)
            {
                const int node_idx = ObjectPoolFindOrCreateIndex(editor->Nodes, id);
                GImNodes.CurrentNodeIdx = node_idx;
                ImNodeData& node = editor->Nodes.Pool[node_idx];
                node.Id = id;
            }
            else if (sscanf(line, "origin=%i,%i", &x, &y) == 2)
            {
                ImNodeData& node = editor->Nodes.Pool[GImNodes.CurrentNodeIdx];
                node.Origin = ImVec2((float)x, (float)y);
            }
        }

        void EditorLineHandler(ImNodesEditorContext  *editor, const char* const line)
        {
            (void)sscanf(line, "panning=%f,%f", &editor->Panning.x, &editor->Panning.y);
        }



    // |||||||||||||| From imnodes ||||||||||||||||||||

    // [SECTION] bezier curve helpers
private:
    struct CubicBezier
    {
        ImVec2 P0, P1, P2, P3;
        int    NumSegments;
    };

    inline ImVec2 EvalCubicBezier(
        const float   t,
        const ImVec2& P0,
        const ImVec2& P1,
        const ImVec2& P2,
        const ImVec2& P3)
    {
        // B(t) = (1-t)**3 p0 + 3(1 - t)**2 t P1 + 3(1-t)t**2 P2 + t**3 P3

        const float u = 1.0f - t;
        const float b0 = u * u * u;
        const float b1 = 3 * u * u * t;
        const float b2 = 3 * u * t * t;
        const float b3 = t * t * t;
        return ImVec2(
            b0 * P0.x + b1 * P1.x + b2 * P2.x + b3 * P3.x,
            b0 * P0.y + b1 * P1.y + b2 * P2.y + b3 * P3.y);
    }

    // Calculates the closest point along each bezier curve segment.
    ImVec2 GetClosestPointOnCubicBezier(const int num_segments, const ImVec2& p, const CubicBezier& cb)
    {
        IM_ASSERT(num_segments > 0);
        ImVec2 p_last = cb.P0;
        ImVec2 p_closest;
        float  p_closest_dist = FLT_MAX;
        float  t_step = 1.0f / (float)num_segments;
        for (int i = 1; i <= num_segments; ++i)
        {
            ImVec2 p_current = EvalCubicBezier(t_step * i, cb.P0, cb.P1, cb.P2, cb.P3);
            ImVec2 p_line = ImLineClosestPoint(p_last, p_current, p);
            float  dist = ImLengthSqr(p - p_line);
            if (dist < p_closest_dist)
            {
                p_closest = p_line;
                p_closest_dist = dist;
            }
            p_last = p_current;
        }
        return p_closest;
    }

    inline float GetDistanceToCubicBezier(
        const ImVec2& pos,
        const CubicBezier& cubic_bezier,
        const int          num_segments)
    {
        const ImVec2 point_on_curve = GetClosestPointOnCubicBezier(num_segments, pos, cubic_bezier);

        const ImVec2 to_curve = point_on_curve - pos;
        return ImSqrt(ImLengthSqr(to_curve));
    }

    inline ImRect GetContainingRectForCubicBezier(const CubicBezier& cb)
    {
        const ImVec2 min = ImVec2(ImMin(cb.P0.x, cb.P3.x), ImMin(cb.P0.y, cb.P3.y));
        const ImVec2 max = ImVec2(ImMax(cb.P0.x, cb.P3.x), ImMax(cb.P0.y, cb.P3.y));

        const float hover_distance = GImNodes.Style.LinkHoverDistance;

        ImRect rect(min, max);
        rect.Add(cb.P1);
        rect.Add(cb.P2);
        rect.Expand(ImVec2(hover_distance, hover_distance));

        return rect;
    }

    inline CubicBezier GetCubicBezier(
        ImVec2                     start,
        ImVec2                     end,
        const ImNodesAttributeType start_type,
        const float                line_segments_per_length)
    {
        assert(
            (start_type == ImNodesAttributeType_Input) || (start_type == ImNodesAttributeType_Output));
        if (start_type == ImNodesAttributeType_Input)
        {
            ImSwap(start, end);
        }

        const float  link_length = ImSqrt(ImLengthSqr(end - start));
        const ImVec2 offset = ImVec2(0.25f * link_length, 0.f);
        CubicBezier  cubic_bezier;
        cubic_bezier.P0 = start;
        cubic_bezier.P1 = start + offset;
        cubic_bezier.P2 = end - offset;
        cubic_bezier.P3 = end;
        cubic_bezier.NumSegments = ImMax(static_cast<int>(link_length * line_segments_per_length), 1);
        return cubic_bezier;
    }

    inline float EvalImplicitLineEq(const ImVec2& p1, const ImVec2& p2, const ImVec2& p)
    {
        return (p2.y - p1.y) * p.x + (p1.x - p2.x) * p.y + (p2.x * p1.y - p1.x * p2.y);
    }

    inline int Sign(float val) { return int(val > 0.0f) - int(val < 0.0f); }

    inline bool RectangleOverlapsLineSegment(const ImRect& rect, const ImVec2& p1, const ImVec2& p2)
    {
        // Trivial case: rectangle contains an endpoint
        if (rect.Contains(p1) || rect.Contains(p2))
        {
            return true;
        }

        // Flip rectangle if necessary
        ImRect flip_rect = rect;

        if (flip_rect.Min.x > flip_rect.Max.x)
        {
            ImSwap(flip_rect.Min.x, flip_rect.Max.x);
        }

        if (flip_rect.Min.y > flip_rect.Max.y)
        {
            ImSwap(flip_rect.Min.y, flip_rect.Max.y);
        }

        // Trivial case: line segment lies to one particular side of rectangle
        if ((p1.x < flip_rect.Min.x && p2.x < flip_rect.Min.x) ||
            (p1.x > flip_rect.Max.x && p2.x > flip_rect.Max.x) ||
            (p1.y < flip_rect.Min.y && p2.y < flip_rect.Min.y) ||
            (p1.y > flip_rect.Max.y && p2.y > flip_rect.Max.y))
        {
            return false;
        }

        const int corner_signs[4] = {
            Sign(EvalImplicitLineEq(p1, p2, flip_rect.Min)),
            Sign(EvalImplicitLineEq(p1, p2, ImVec2(flip_rect.Max.x, flip_rect.Min.y))),
            Sign(EvalImplicitLineEq(p1, p2, ImVec2(flip_rect.Min.x, flip_rect.Max.y))),
            Sign(EvalImplicitLineEq(p1, p2, flip_rect.Max)) };

        int sum = 0;
        int sum_abs = 0;

        for (int i = 0; i < 4; ++i)
        {
            sum += corner_signs[i];
            sum_abs += abs(corner_signs[i]);
        }

        // At least one corner of rectangle lies on a different side of line segment
        return abs(sum) != sum_abs;
    }

    inline bool RectangleOverlapsBezier(const ImRect& rectangle, const CubicBezier& cubic_bezier)
    {
        ImVec2 current =
            EvalCubicBezier(0.f, cubic_bezier.P0, cubic_bezier.P1, cubic_bezier.P2, cubic_bezier.P3);
        const float dt = 1.0f / cubic_bezier.NumSegments;
        for (int s = 0; s < cubic_bezier.NumSegments; ++s)
        {
            ImVec2 next = EvalCubicBezier(
                static_cast<float>((s + 1) * dt),
                cubic_bezier.P0,
                cubic_bezier.P1,
                cubic_bezier.P2,
                cubic_bezier.P3);
            if (RectangleOverlapsLineSegment(rectangle, current, next))
            {
                return true;
            }
            current = next;
        }
        return false;
    }

    inline bool RectangleOverlapsLink(
        const ImRect& rectangle,
        const ImVec2& start,
        const ImVec2& end,
        const ImNodesAttributeType start_type)
    {
        // First level: simple rejection test via rectangle overlap:

        ImRect lrect = ImRect(start, end);
        if (lrect.Min.x > lrect.Max.x)
        {
            ImSwap(lrect.Min.x, lrect.Max.x);
        }

        if (lrect.Min.y > lrect.Max.y)
        {
            ImSwap(lrect.Min.y, lrect.Max.y);
        }

        if (rectangle.Overlaps(lrect))
        {
            // First, check if either one or both endpoinds are trivially contained
            // in the rectangle

            if (rectangle.Contains(start) || rectangle.Contains(end))
            {
                return true;
            }

            // Second level of refinement: do a more expensive test against the
            // link

            const CubicBezier cubic_bezier =
                GetCubicBezier(start, end, start_type, GImNodes.Style.LinkLineSegmentsPerLength);
            return RectangleOverlapsBezier(rectangle, cubic_bezier);
        }

        return false;
    }


private:

/*
    const char* SaveCurrentEditorStateToIniString(size_t* const data_size)
    {
       // return SaveEditorStateToIniString(&EditorContextGet(), data_size);
        return SaveEditorStateToIniString(EditorContextGet(), data_size);
    }

    const char* SaveEditorStateToIniString(
        const ImNodesEditorContext *editor_ptr,
        size_t* const                     data_size)
    {
        assert(editor_ptr != NULL);
        const ImNodesEditorContext  *editor = editor_ptr;

        GImNodes.TextBuffer.clear();
        // TODO: check to make sure that the estimate is the upper bound of element
        GImNodes.TextBuffer.reserve(64 * editor->Nodes.Pool.size());

        GImNodes.TextBuffer.appendf(
            "[editor]\npanning=%i,%i\n", (int)editor->Panning.x, (int)editor->Panning.y);

        for (int i = 0; i < editor->Nodes.Pool.size(); i++)
        {
            if (editor->Nodes.InUse[i])
            {
                const ImNodeData& node = editor->Nodes.Pool[i];
                GImNodes.TextBuffer.appendf("\n[node.%d]\n", node.Id);
                GImNodes.TextBuffer.appendf("origin=%i,%i\n", (int)node.Origin.x, (int)node.Origin.y);
            }
        }

        if (data_size != NULL)
        {
            *data_size = GImNodes.TextBuffer.size();
        }

        return GImNodes.TextBuffer.c_str();
    }

    void LoadCurrentEditorStateFromIniString(const char* const data, const size_t data_size)
    {
        LoadEditorStateFromIniString(EditorContextGet(), data, data_size);
    }

    void LoadEditorStateFromIniString(
        ImNodesEditorContext* const editor_ptr,
        const char* const           data,
        const size_t                data_size)
    {
        if (data_size == 0u)
        {
            return;
        }

        ImNodesEditorContext  *editor = editor_ptr == NULL ? EditorContextGet() : editor_ptr;

        char* buf = (char*)ImGui::MemAlloc(data_size + 1);
        const char* buf_end = buf + data_size;
        memcpy(buf, data, data_size);
        buf[data_size] = 0;

        //void (*line_handler)(ImNodesEditorContext&, const char*);
        void (*line_handler)(ImNodesEditorContext, const char*);
        line_handler = NULL;
        char* line_end = NULL;
        for (char* line = buf; line < buf_end; line = line_end + 1)
        {
            while (*line == '\n' || *line == '\r')
            {
                line++;
            }
            line_end = line;
            while (line_end < buf_end && *line_end != '\n' && *line_end != '\r')
            {
                line_end++;
            }
            line_end[0] = 0;

            if (*line == ';' || *line == '\0')
            {
                continue;
            }

            if (line[0] == '[' && line_end[-1] == ']')
            {
                line_end[-1] = 0;
                if (strncmp(line + 1, "node", 4) == 0)
                {
                    line_handler = NodeLineHandler;
                }
                else if (strcmp(line + 1, "editor") == 0)
                {
                    line_handler = EditorLineHandler;
                }
            }

            if (line_handler != NULL)
            {
                line_handler(editor, line);
            }
        }
        ImGui::MemFree(buf);
    }

    void SaveCurrentEditorStateToIniFile(const char* const file_name)
    {
        SaveEditorStateToIniFile(&EditorContextGet(), file_name);
    }

    void SaveEditorStateToIniFile(const ImNodesEditorContext* const editor, const char* const file_name)
    {
        size_t      data_size = 0u;
        const char* data = SaveEditorStateToIniString(editor, &data_size);
        FILE* file = ImFileOpen(file_name, "wt");
        if (!file)
        {
            return;
        }

        fwrite(data, sizeof(char), data_size, file);
        fclose(file);
    }

    void LoadCurrentEditorStateFromIniFile(const char* const file_name)
    {
        LoadEditorStateFromIniFile(&EditorContextGet(), file_name);
    }

    void LoadEditorStateFromIniFile(ImNodesEditorContext* const editor, const char* const file_name)
    {
        size_t data_size = 0u;
        char* file_data = (char*)ImFileLoadToMemory(file_name, "rb", &data_size);

        if (!file_data)
        {
            return;
        }

        LoadEditorStateFromIniString(editor, file_data, data_size);
        ImGui::MemFree(file_data);
    }
*/

};
/*
class node_editor_basis_class {
public:
    ImNodesContext* nodes_context = NULL;

protected:
    graph_class graph;
    int         root_node_id_;

    bool no_node_element_is_hovered_over = true;

    int hover_node_id, hover_link_id, hover_pin_id;
    int current_selected_node_id = -1;
    int num_selected_nodes = 0;

    int current_selected_link_id = -1;//*****
    int num_selected_links = 0;//*****

    bool inside_editor_canvas(ImVec2 location) {
        if (location.x >= nodes_context->CanvasRectScreenSpace.Min.x &&
            location.x <= nodes_context->CanvasRectScreenSpace.Max.x &&
            location.y >= nodes_context->CanvasRectScreenSpace.Min.y &&
            location.y <= nodes_context->CanvasRectScreenSpace.Max.y)
            return true;
        else
            return false;
    }

    bool no_element_hovered_over() {
        if (ImNodes::IsNodeHovered(&hover_node_id) ||
            ImNodes::IsLinkHovered(&hover_link_id) ||
            ImNodes::IsPinHovered(&hover_pin_id))
            return false;
        else
            return true;
    }

private:
};
*/