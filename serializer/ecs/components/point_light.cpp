#include "point_light.h"

namespace serializer::ecs::point_light {
    void save(writer& writer, ::ecs::entity::entity& entity) {

        auto* light = entity.get_point_light();
        if ( !light ) {
            writer.write(static_cast<unsigned int>(0));
            return;
        }

        writer.write(static_cast<unsigned int>(1));

        glm::vec3 diffuse = light->get_diffuse();
        glm::vec3 specular = light->get_specular();
        glm::vec3 ambient = light->get_ambient();

        writer.write(glm::value_ptr(ambient), sizeof(ambient) / sizeof(float));
        writer.write(glm::value_ptr(diffuse), sizeof(diffuse) / sizeof(float));
        writer.write(glm::value_ptr(specular), sizeof(specular) / sizeof(float));

        writer.write(light->get_constant());
        writer.write(light->get_linear());
        writer.write(light->get_quadratic());
    }

    void load(reader& reader, ::ecs::entity::entity& entity) {
        unsigned int exists;
        if ( !reader.is_in_range<unsigned int>())
            return;
        reader.read(exists);

        if ( !exists )
            return;

        glm::vec3 ambient, diffuse, specular;
        float constant, linear, quadratic;

        reader.read(glm::value_ptr(ambient));
        reader.read(glm::value_ptr(diffuse));
        reader.read(glm::value_ptr(specular));
        reader.read(constant);
        reader.read(linear);
        reader.read(quadratic);

        entity.create_point_light();
        auto* point_light = entity.get_point_light();
        point_light->set_ambient(ambient);
        point_light->set_diffuse(diffuse);
        point_light->set_specular(specular);
        point_light->set_constant(constant);
        point_light->set_linear(linear);
        point_light->set_quadratic(quadratic);
    }
}