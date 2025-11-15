#include "textbox_popup.h"
#include "imgui.h"

namespace editor::helpers::textbox::popup {
    namespace {
        bool initialized = false;
        std::string _result;
        std::string _preview_text;
        std::string _popup_name;
        bool show_error_popup = false;
        bool was_cancelled = false;

        void finalize()
        {
            initialized = false;
            show_error_popup = false;
        }
    }

    void initialize(const std::string& preview_text,const std::string& popup_name)
    {
        _preview_text = preview_text;
        _popup_name = popup_name;
        initialized = true;
        _result.clear();
        show_error_popup = false;
        was_cancelled = false;
    }

    void open()
    {
        ImGui::SetNextWindowSize(ImVec2(256, 128));
        ImGui::OpenPopup(_popup_name.c_str());
    }

    void update()
    {
        if ( !initialized )
            return;

        if ( ImGui::BeginPopupModal(_popup_name.c_str()) )
        {
            char buf[255];
            strcpy(buf, _result.c_str());
            ImGui::InputTextWithHint("##input", _preview_text.c_str(), buf, 255);
            _result = buf;

            float content_height = ImGui::GetContentRegionAvail().y - 30;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + content_height);

            float button_width = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.x) / 2;

            if( ImGui::Button("Confirm" ,ImVec2(button_width, 25)) )
            {

                if (!_result.empty())
                {
                    finalize();
                    ImGui::CloseCurrentPopup();
                }
                else
                {
                    show_error_popup = true;
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Close", ImVec2(button_width, 25)))
            {
                was_cancelled = true;
                finalize();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (show_error_popup)
        {
            ImGui::OpenPopup("Error");
        }

        if (ImGui::BeginPopupModal("Error", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("Name cannot be empty!");
            ImGui::Spacing();

            if (ImGui::Button("Close", ImVec2(-1, 25)))
            {
                show_error_popup = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    std::string result()
    {
        return _result;
    }

    bool finished()
    {
        return !initialized && !was_cancelled;
    }
}