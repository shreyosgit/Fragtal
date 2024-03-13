#include "Theme.h"

namespace UITheme {

    void SetUITheme() {

        auto& style = ImGui::GetStyle();
        auto& colors = ImGui::GetStyle().Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.0980f, 0.0980f, 0.0980f, 1.0f); // Window Background
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.2352f, 0.2352f, 0.2352f, 1.0f);
        colors[ImGuiCol_Border] = ImVec4(0.1372f, 0.1372f, 0.1372f, 1.0f); // Divider Color

        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);

        colors[ImGuiCol_FrameBg] = ImVec4(0.2352f, 0.2352f, 0.2352f, 1.0f); // Button Colors
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1882f, 0.1960f, 0.2f, 1.0f); // Button Colors Hover

        colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352f, 0.2352f, 0.2352f, 1.0f); // Menu Button Click

        colors[ImGuiCol_TitleBg] = ImVec4(0.0980f, 0.0980f, 0.0980f, 1.0f); // Title Bar Inactive
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.0980f, 0.0980f, 0.0980f, 1.0f); // Title Bar Active

        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);

        colors[ImGuiCol_MenuBarBg] = ImVec4(0.0980f, 0.0980f, 0.0980f, 1.0f); // Main Window Menubar

        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

        colors[ImGuiCol_Button] = ImVec4(0.2352f, 0.2352f, 0.2352f, 1.0f); // Buttons
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.1882f, 0.1960f, 0.2f, 1.0f); // Button Hover
        colors[ImGuiCol_ButtonActive] = ImVec4(0.2352f, 0.2352f, 0.2352f, 1.0f); // Button Clicks
        colors[ImGuiCol_Header] = ImVec4(0.0980f, 0.0980f, 0.0980f, 1.0f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.1882f, 0.1960f, 0.2f, 1.0f); // Header Hovered
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.470f, 0.470f, 0.470f, 1.0f }; // Header Active
        colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];

        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3552f, 0.3552f, 0.3552f, 1.0f); // Separated Window Corner Color
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.3552f, 0.3552f, 0.3552f, 1.0f); // Separated Window Corner Color Active

        colors[ImGuiCol_ResizeGrip] = ImVec4(0.3552f, 0.3552f, 0.3552f, 1.0f); // Window Corner Resize Grip
        colors[ImGuiCol_ResizeGripHovered] = colors[ImGuiCol_SeparatorHovered]; // Border Separator Hover
        colors[ImGuiCol_ResizeGripActive] = colors[ImGuiCol_SeparatorActive]; // Border Separator Active

        colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
        colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
        colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
        colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
        colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);

        colors[ImGuiCol_DockingPreview] = colors[ImGuiCol_HeaderActive] * ImVec4(1.0f, 1.0f, 1.0f, 0.75f); // Docking Preview
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f); // Main Window Background

        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}