#pragma once
#include "core/serializer.h"
#include "engine/content/mesh.h"

namespace serializer::content::mesh {
    std::string get_detail(unsigned int mesh_id);
    std::string get_base(unsigned int mesh_id);

    void load_detail(reader& detail_reader, ::content::mesh::mesh& mesh);
    void load_base(reader& base_reader, ::content::mesh::mesh& mesh);
}