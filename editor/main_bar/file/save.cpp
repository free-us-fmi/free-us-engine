#include "save.h"
#include "serializer/project/active_project.h"

namespace editor::main_menu::file::save {
    void run() {
        serializer::project::active::save();
    }
}