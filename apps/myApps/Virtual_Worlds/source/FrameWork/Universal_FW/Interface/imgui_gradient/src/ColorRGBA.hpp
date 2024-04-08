#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h> // Include ImVec4

// Need the following as imgui does not have == operator
#include <FrameWork/Kernels/ImGuiEx/ImGui_vec_operators.h>

namespace ImGG {

/// sRGB, Straight Alpha
using ColorRGBA = ImVec4;

} // namespace ImGG