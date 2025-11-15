#pragma once 
#include "ECS/ecs.h"

namespace editor::transform 
{
void update(ecs::components::transform::transform* transform, const std::string& entity_name);
}
