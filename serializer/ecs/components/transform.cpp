#include "transform.h"
#include "engine/utility/unmove.h"

namespace serializer::ecs::transform {
    void save(writer& writer, ::ecs::entity::entity& entity) {
        auto* transform = entity.get_transform();

        if ( !transform ) {
            writer.write(static_cast<unsigned int>(0));
            return;
        }

        writer.write(static_cast<unsigned int>(1));

        glm::vec3 position = transform->get_position();
        glm::vec3 rotation = transform->get_rotation();
        glm::vec3 scale = transform->get_scale();

        writer.write(glm::value_ptr(position),sizeof(position) / sizeof(float));
        writer.write(glm::value_ptr(rotation),sizeof(rotation) / sizeof(float));
        writer.write(glm::value_ptr(scale),sizeof(scale) / sizeof(float));

    }

    void load(reader& reader, ::ecs::entity::entity& entity) {

        unsigned int exists;
        if ( !reader.is_in_range<unsigned int>())
            return;

        reader.read(exists);

        if ( !exists )
            return;

        auto* transform = entity.get_transform();
        glm::vec3 position, rotation, scale;
        reader.read(glm::value_ptr(position));
        reader.read(glm::value_ptr(rotation));
        reader.read(glm::value_ptr(scale));
        transform->set_position(position);
        transform->set_rotation(rotation);
        transform->set_scale(scale);
    }

}