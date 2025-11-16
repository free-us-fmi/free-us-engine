#include "file.h"
#include "imgui.h"
#include "files/file_popup.h"
#include "assets/AssetsPath.h"
#include "materials/new_material_popup.h"
#include "utility/helpers/textbox/textbox_popup.h"
#include "serializer/project/active_project.h"
#include "engine/ECS/ecs.h"
#include "engine/content/scene.h"
#include "unload.h"
#include "save_as.h"
#include "load.h"
#include "new_project.h"
#include "save.h"

namespace editor::main_menu::file {

    void update() {
        save_as::update();
        load::update();
    }

    void show() {
        if ( ImGui::MenuItem("New"))
            new_project::run();
        ImGui::Separator();

        if ( ImGui::MenuItem("Save as") )
            save_as::run();
        ImGui::Separator();

        if ( ImGui::MenuItem("Save"))
            save::run();
        ImGui::Separator();

        if ( ImGui::MenuItem("Unload"))
            unload::run();
        ImGui::Separator();

        if ( ImGui::MenuItem("Open"))
            load::run();
    }
}
