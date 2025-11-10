#include "main_menu.h"
#include "imgui.h"
#include "files/file_browser.h"
#include "file/file.h"

namespace editor::main_menu {

    namespace {

    }

    void update() {
        ImGui::BeginMainMenuBar();

        if ( ImGui::BeginMenu("File")) {
            file::show();
            ImGui::EndMenu();
        }
        file::update();

        ImGui::EndMainMenuBar();
    }
}