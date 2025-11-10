#include "load.h"
#include <filesystem>
#include <string>
#include <cassert>

#include "unload.h"
#include "files/file_popup.h"
#include "engine/assets/AssetsPath.h"
#include "serializer/project/active_project.h"
#include "utility/helpers/textbox/textbox_popup.h"
namespace editor::main_menu::file::load {

    enum class state {
        selecting_project,
        loading,
        none
    };

    namespace {

        auto loading_state = state::none;
        std::filesystem::path project_path;
        std::string project_name;

        void update_file_selection() {
            static bool popup = false;
            if ( !popup ) {
                    file_browser::popup::initialize(assets::GetAssetsPath(), true, false);

                file_browser::popup::open();
                popup = true;
            }

            if ( !file_browser::popup::finished() )
                file_browser::popup::update();
            else {
                project_path = file_browser::popup::get_selected_file_path();
                popup = false;
                loading_state = state::loading;
            }

        }
    }

    void update_saving() {
        serializer::project::active::load(project_path);
        loading_state = state::none;
    }

    void begin() {
        loading_state = state::selecting_project;
    }

    void update() {
        switch ( loading_state ) {
            case state::selecting_project:
                update_file_selection();
                break;
            case state::loading:
                update_saving();
                break;
            case state::none:
                break;
        }
    }

    void run() {
        assert(loading_state == state::none );
        unload::run();
        begin();
    }

}
