#include "geometry.h"

#include "content/scene.h"
#include "managers/ProgramManager.h"

namespace serializer::ecs::geometry {
    void save(writer& writer, ::ecs::entity::entity& entity) {
        auto* geometry = entity.get_geometry();
        if ( !geometry ) {
            writer.write(static_cast<unsigned int>(0));
            return;
        }

        writer.write(static_cast<unsigned int>(1));

        std::string scene_name = geometry->get_model_name();

        writer.write(scene_name);
    }

    void load(reader& reader, ::ecs::entity::entity& entity) {
        unsigned int exists;
        if ( !reader.is_in_range<unsigned int>())
            return;
        reader.read(exists);

        if ( !exists )
            return;

        std::string scene_name;
        reader.read(scene_name);

        utl::vector<::programs::program_id> program_ids;
        program_ids.emplace_back(::programs::GetProgramId("default"));
        entity.create_geometry(scene_name, program_ids);
    }
}
