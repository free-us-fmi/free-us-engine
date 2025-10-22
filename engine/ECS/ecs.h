#pragma once
#include "utility/vector.h"
#include "entity.h"

namespace ecs 
{

utl::vector<entity::entity, false, 1024>& get_entity_vector();

constexpr bool is_valid(entity::entity_id id );

entity::entity_id create_entity(const std::string& name = "unnamed entity");
void remove_entity(entity::entity_id id);

entity::entity* get_entity(entity::entity_id id);

void draw();
void update();
}
