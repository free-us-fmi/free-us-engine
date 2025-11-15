#include "raymarched_geometry.h"

#include "ECS/ecs.h"
#include "managers/ProgramManager.h"
#include "raymarcher/renderer.h"
#include "shaders/program.h"
#include "utility/vector.h"

namespace ecs::components::raymarched_geometry {
    namespace {
        utl::vector<raymarched_geometry, false, 1024> components;
        utl::vector<unsigned int> generations;
    }

    raymarched_geometry_id create_raymarched_geometry(entity::entity_id entity)
    {
        unsigned int index = components.emplace_tombstone(entity);
        if ( index >= generations.size() )
            generations.emplace_back(0);

        raymarched_geometry_id id = id::set_generation(index, generations[index]);

        return id;
    }

    void remove_raymarched_geometry(raymarched_geometry_id id)
    {
        assert(id::generation(id) == generations[id::index(id)]);
        components.erase(components.internal_begin() + id::index(id));
        ++generations[id::index(id)];
    }

    raymarched_geometry* get_raymarched_geometry(raymarched_geometry_id id )
    {
        if ( !is_valid(id) )
            return nullptr;

        return &components[id::index(id)];
    }

    bool is_valid(raymarched_geometry_id id)
    {
        if ( id == id::invalid_id )
            return false;
        return id::generation(id) == generations[id::index(id)];
    }

    void update() {
        auto* program = programs::GetProgram(raymarching::get_program());
        if (!program or !program->linked())
            return;
        int component_number = 0;
        for ( auto& component : components ) {
            auto* transform = ecs::get_entity(component.get_entity_id())->get_transform();

            program->SetUniform3fv("objects[" + std::to_string(component_number) + "].scale", transform->get_scale());
            program->SetUniform3fv("objects[" + std::to_string(component_number) + "].rotation", transform->get_rotation());
            program->SetUniform3fv("objects[" + std::to_string(component_number) + "].position", transform->get_position());
            program->SetUniform1i("objects[" + std::to_string(component_number) + "].type", static_cast<int>(component.get_model_type()));
            component_number++;
        }
        program->SetUniform1i("no_objects", component_number);
    }
}
