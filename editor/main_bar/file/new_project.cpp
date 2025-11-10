#include "new_project.h"

#include "save.h"
#include "save_as.h"
#include "unload.h"
namespace editor::main_menu::file::new_project {
    void run() {
        save::run();
        unload::run();
        save_as::run();
    }
}