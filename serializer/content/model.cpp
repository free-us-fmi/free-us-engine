#include "model.h"
#include "mesh.h"
#include "core/serializer.h"
#include "glm/glm.hpp"
#include <map>

namespace serializer::content::model {

    namespace {
        utl::vector<unsigned int> mesh_positions;
        utl::vector<unsigned int> indices_to_meshes;
        std::map<unsigned int, unsigned int> meshes;

        utl::vector<::content::mesh::mesh> loading_meshes_cache;

        unsigned int count = 0;

        void update_mesh_order(::content::model::model* model) {

            mesh_positions.clear();
            meshes.clear();
            indices_to_meshes.clear();
            count = 0;

            for ( unsigned int mesh_id : model->_meshes ) {
                if ( !meshes.contains(mesh_id) ) {
                    meshes[mesh_id] = count;
                    indices_to_meshes.emplace_back(mesh_id);
                    ++count;
                }
                mesh_positions.emplace_back(meshes[mesh_id]);
            }
        }

        void solve_model(::content::model::model& model) {
            for ( unsigned int i = 0; i < loading_meshes_cache.size(); ++i )
                meshes[i] = ::content::mesh::add_mesh(loading_meshes_cache[i]);

            for ( auto& mesh : model._meshes )
                mesh = meshes[mesh];

            loading_meshes_cache.clear();
        }
    }

    std::string get_detail(unsigned int model_id) {
        ::content::model::model *model  = ::content::model::get_model(model_id);
        update_mesh_order(model);

        std::string result;
        writer writer;

        for ( unsigned int i = 0; i < count; ++i )
            result += mesh::get_detail(indices_to_meshes[i]);

        return result;
    }

    void load_detail(reader& detail_reader, ::content::model::model& model) {
        for ( auto& mesh : loading_meshes_cache )
            mesh::load_detail(detail_reader, mesh);

        solve_model(model);
    }

    std::string get_base(unsigned int model_id) {
        ::content::model::model *model  = ::content::model::get_model(model_id);
        update_mesh_order(model);

        std::string result;
        writer writer;
        writer.write(count);
        result += writer.get_buffer();
        writer.clear();

        for ( unsigned int i = 0; i < count; ++i )
            result += mesh::get_base(indices_to_meshes[i]);

        writer.write(mesh_positions.data(), mesh_positions.size());
        writer.write(glm::value_ptr(model->_local_model), 16);
        result += writer.get_buffer();

        return result;
    }

    void load_base(reader& base_reader, ::content::model::model& model) {

        auto& meshes_ids = model._meshes;
        unsigned int meshes_count;
        base_reader.read(meshes_count);

        for ( unsigned int i = 0; i < meshes_count; ++i ) {
            loading_meshes_cache.emplace_back();
            auto& last_mesh = loading_meshes_cache[i];

            mesh::load_base(base_reader, last_mesh);
        }

        unsigned int no_meshes;
        base_reader.read(no_meshes);

        for ( unsigned int i = 0; i < no_meshes; ++i ) {
            unsigned int local_mesh_id;
            base_reader.read(local_mesh_id);
            meshes_ids.emplace_back(local_mesh_id);
        }

        base_reader.read(glm::value_ptr(model._local_model));
    }


}
