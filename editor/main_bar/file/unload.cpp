#include "unload.h"

#include "engine/content/scene.h"
#include "engine/ECS/ecs.h"
#include "managers/MaterialManager.h"

namespace editor::main_menu::file::unload {
    void run() {
        ::ecs::unload();
        ::content::scene::unload();
        ::materials::UnloadMaterials();
    }
}
