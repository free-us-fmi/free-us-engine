#include "textbox_popup.h"
#include "imgui.h"

namespace editor::helpers::textbox::popup {
    namespace {


        bool initialized = false;
        std::string _result;
        std::string _preview_text;
        std::string _popup_name;

        void finalize()
        {
            initialized = false;
        }
    }

    void initialize(const std::string& preview_text,const std::string& popup_name)
    {
        _preview_text = preview_text;
        _popup_name = popup_name;
        initialized = true;
        _result.clear();
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
            ImGui::InputText(_preview_text.c_str(), buf, 255);
            _result = buf;

            if( ImGui::Button("confirm") )
            {
                finalize();
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
        return !initialized;
    }
}