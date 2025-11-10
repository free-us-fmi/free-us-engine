#include "mesh.h"
#include "core/serializer.h"
#include "managers/MaterialManager.h"
#include "materials/materials.h"

namespace serializer::content::mesh {

    std::string get_detail(unsigned int mesh_id) {
        ::content::mesh::mesh* mesh = ::content::mesh::get_mesh(mesh_id);
        ::materials::material* mat = ::materials::GetMaterial(mesh->_material);
        writer detail_writer;
        if ( mat )
           detail_writer.write(mat->_name);
        else detail_writer.write(0);
        return detail_writer.get_buffer();
    }

    void load_detail(reader& detail_reader, ::content::mesh::mesh& mesh) {
        std::string material_name;
        detail_reader.read(material_name);
        mesh.set_material(material_name);
    }

    std::string get_base(unsigned int mesh_id) {
        ::content::mesh::mesh* mesh = ::content::mesh::get_mesh(mesh_id);

        writer writer;

        auto& vertices = mesh->_vertices;
        auto& indices = mesh->_indices;

        writer.write(vertices.size());
        for ( auto& vertex : vertices) {
            writer.write(glm::value_ptr(vertex.normal), sizeof(vertex.normal) / sizeof(float));
            writer.write(glm::value_ptr(vertex.position), sizeof(vertex.position) / sizeof(float));
            writer.write(glm::value_ptr(vertex.uv), sizeof(vertex.uv) / sizeof(float));
        }

        writer.write(indices.data(), indices.size());

        return writer.get_buffer();
    }

    void load_base(reader& base_reader, ::content::mesh::mesh& mesh) {
        auto& vertices = mesh._vertices;
        auto& indices = mesh._indices;

        unsigned int vertices_count;
        base_reader.read(vertices_count);
        for ( unsigned int i = 0; i < vertices_count; i++) {
            vertices.emplace_back();
            auto& vertex = vertices[i];

            auto& normal = vertices[i].normal;
            auto& position = vertices[i].position;
            auto& uv = vertices[i].uv;

            base_reader.read(glm::value_ptr(normal));
            base_reader.read(glm::value_ptr(position));
            base_reader.read(glm::value_ptr(uv));
        }

        unsigned int indices_count;
        base_reader.read(indices_count);
        for ( unsigned int i = 0; i < indices_count; i++) {
            indices.emplace_back();
            base_reader.read(indices[i]);
        }
    }
}
