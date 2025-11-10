#include "save.h"

#include <cassert>
#include <filesystem>
#include <string>

#include "engine/assets/AssetsPath.h"
#include "files/file_popup.h"
#include "serializer/project/active_project.h"
#include "utility/helpers/textbox/textbox_popup.h"

namespace editor::main_menu::file::save_as {

    enum class state {
        selecting_file,
        naming,
        saving,
        none
    };

    namespace {

        auto saving_state = state::none;
        std::filesystem::path project_path;
        std::string project_name;

        void update_file_selection() {
            static bool popup = false;
            if ( !popup ) {
                file_browser::popup::initialize(assets::GetAssetsPath(), true, true);

                file_browser::popup::open();
                popup = true;
            }

            if ( !file_browser::popup::finished() )
                file_browser::popup::update();
            else {
                project_path = file_browser::popup::get_selected_file_path();
                popup = false;
                saving_state = state::naming;
            }

        }
    }

    void update_naming() {
        static bool popup = false;
        if ( !popup ) {
            popup = true;
            helpers::textbox::popup::initialize("save project", "proect name");
            helpers::textbox::popup::open();
        }

        if ( !helpers::textbox::popup::finished() )
            helpers::textbox::popup::update();
        else {
            popup = false;
            project_name = helpers::textbox::popup::result();
            saving_state = state::saving;

        }
    }

    void update_saving() {
        serializer::project::active::save_as(project_path, project_name);
        saving_state = state::none;
    }

    void update() {
        switch ( saving_state ) {
            case state::selecting_file:
                update_file_selection();
                break;
            case state::naming:
                update_naming();
                break;
            case state::saving:
                update_saving();
                break;
            case state::none:
                break;
        }
    }

    void run() {
        assert(saving_state == state::none );
        saving_state = state::selecting_file;
    }

}
