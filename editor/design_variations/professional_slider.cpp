#include "professional_slider.h"

#include <string>

#include "professional_style.h"

namespace editor::professional_slider
{
    void ProfessionalSlider(const char* label, float* value, float min_val, float max_val, const char* unit)
    {
        ImGui::BeginGroup();
        {
            ImGui::PushID(label);

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));

            ImGui::TextDisabled("%s", label);
            ImGui::SameLine(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("000.00").x);
            ImGui::Text("%.2f%s", *value, unit);

            ImGui::PopStyleVar();

            ImGui::SetNextItemWidth(-1);

            std::string slider_id = "##slider_";
            slider_id += label;
            ImGui::SliderScalar(slider_id.c_str(), ImGuiDataType_Float, value, &min_val, &max_val, "", ImGuiSliderFlags_NoInput);

            ImGui::PopID();
        }
        ImGui::EndGroup();
    }
}