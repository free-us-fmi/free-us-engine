#pragma once
#include "core/serializer.h"
#include "engine/ECS/entity.h"

namespace serializer::ecs::transform {

    void save(writer& writer, ::ecs::entity::entity& entity);
    void load(reader& reader, ::ecs::entity::entity& entity);

}
    