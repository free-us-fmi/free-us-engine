#include "professional_style.h"

namespace editor::professional_style
{
    void ApplyProfessionalStyle()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, 2.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));

        ImGui::PushStyleColor(ImGuiCol_FrameBg,        ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.12f, 0.12f, 0.12f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  ImVec4(0.16f, 0.16f, 0.16f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrab,     ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.20f, 0.50f, 0.86f, 1.0f));
    }

    void RestoreStyle()
    {
        ImGui::PopStyleColor(5);
        ImGui::PopStyleVar(3);
    }
}