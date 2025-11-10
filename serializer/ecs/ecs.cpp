#include "ecs.h"
#include "engine/ECS/ecs.h"

#include <filesystem>

#include "project/active_project.h"

#include "components/transform.h"
#include "components/point_light.h"
#include "components/geometry.h"
#include "components/instanced_geometry.h"

namespace serializer::ecs {

    namespace {



        void save_entity(writer& scene_writer, ::ecs::entity::entity& entity) {
            scene_writer.write(entity.get_name());

            transform::save(scene_writer, entity);
            point_light::save(scene_writer, entity);
            geometry::save(scene_writer, entity);
            instanced_geometry::save(scene_writer, entity);
        }

        void load_entity(reader& scene_reader) {
            std::string entity_name;
            scene_reader.read(entity_name);

            ::ecs::entity::entity_id entity_id = ::ecs::create_entity(entity_name);
            auto* entity = ::ecs::get_entity(entity_id);

            transform::load(scene_reader, *entity);
            point_light::load(scene_reader, *entity);
            geometry::load(scene_reader, *entity);
            instanced_geometry::load(scene_reader, *entity);
        }

    }

    void save(writer& scene_writer) {
        auto& entities = ::ecs::get_entity_vector();
        scene_writer.write(entities.allocated());
        for ( auto& entity : entities ) {
            save_entity(scene_writer, entity);
        }
    }

    void load(reader& scene_reader) {
        unsigned int count;
        scene_reader.read(count);

        while ( count-- )
            load_entity(scene_reader);
    }
}
