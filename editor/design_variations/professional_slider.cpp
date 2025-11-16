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

            ImGui::SameLine();

            ImGuiID input_id = ImGui::GetID("##input_active");
            static ImGuiID active_input_id = 0;

            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%.2f", *value);

            float input_width = ImGui::CalcTextSize("000.00").x + 8;
            ImGui::SetNextItemWidth(input_width);

            bool is_active = (active_input_id == input_id);

            if (ImGui::InputText("##input", buffer, sizeof(buffer), ImGuiInputTextFlags_CharsDecimal))
            {
                if (ImGui::IsItemActive()) {
                    active_input_id = input_id;
                } else if (is_active) {
                    float new_value = std::atof(buffer);
                    if (new_value >= min_val && new_value <= max_val)
                    {
                        *value = new_value;
                    }
                    active_input_id = 0;
                }

                if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
                {
                    float new_value = std::atof(buffer);
                    if (new_value >= min_val && new_value <= max_val)
                    {
                        *value = new_value;
                    }
                    active_input_id = 0;
                    ImGui::SetKeyboardFocusHere(-1);
                }
            }

            ImGui::SameLine();
            ImGui::Text("%s", unit);

            ImGui::PopStyleVar();

            ImGui::SetNextItemWidth(-1);

            float temp_value = *value;
            if (ImGui::SliderScalar("##slider", ImGuiDataType_Float, &temp_value, &min_val, &max_val, "", ImGuiSliderFlags_NoInput))
            {
                *value = temp_value;
            }

            ImGui::PopID();
        }
        ImGui::EndGroup();
    }
}