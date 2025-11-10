#include "instanced_geometry.h"

#include "content/scene.h"

namespace serializer::ecs::instanced_geometry {
    void save(writer& writer, ::ecs::entity::entity& entity) {
        auto* instanced_geometry = entity.get_instanced_geometry();
        if ( !instanced_geometry ) {
            writer.write(static_cast<unsigned int>(0));
            return;
        }

        writer.write(static_cast<unsigned int>(1));

        std::string scene_name = instanced_geometry->get_model_name();

        writer.write(scene_name);
    }

    void load(reader& reader, ::ecs::entity::entity& entity) {
        unsigned int exists;
        if ( !reader.is_in_range<unsigned int>())
            return;
        reader.read(exists);

        if ( !exists )
            return;

        std::string model_name;
        reader.read(model_name);

        entity.create_instanced_geometry(model_name);
    }
}
