#include "scene.h"
#include "engine/content/scene.h"

#include <filesystem>

#include "model.h"
#include "project/active_project.h"
#include <fstream>
#include <map>

namespace serializer::content::scene {

    namespace {

        constexpr const char* scene_detail_extension = "scd";
        constexpr const char* scene_base_extension = "scb";

        std::filesystem::path get_scenes_path() {
            return project::active::get_project_path() / "models";
        }

        std::filesystem::path get_scene_path(const std::string& scene_uid, const char* extension) {
            assert(scene_uid.find("scene") != std::string::npos);
            std::filesystem::path scene_path =  get_scenes_path() / scene_uid;
            scene_path.replace_extension(extension);

            return scene_path;
        }

        std::filesystem::path get_detail_path(const std::string& scene_uid) {
            return get_scene_path(scene_uid, scene_detail_extension);
        }

        std::filesystem::path get_base_path(const std::string& scene_uid) {
            return get_scene_path(scene_uid, scene_base_extension);
        }

        bool scene_exists(const std::string& scene_uid) {
            return std::filesystem::exists(get_scene_path(scene_uid, scene_detail_extension));
        }
    }

    namespace {
        std::map<unsigned int, unsigned int> models;
        std::map<unsigned int, unsigned int> id_to_models;
        utl::vector<unsigned int> model_positions;
        unsigned int count = 0;

        void map_ids(::content::scene::scene& scene) {

            models.clear();
            id_to_models.clear();
            model_positions.clear();
            count = 0;

            for ( unsigned int model_id : scene._models ) {
                if ( !models.contains(model_id) )
                    id_to_models[count] = model_id, models[model_id] = count++;
                model_positions.emplace_back(models[model_id]);
            }
        }
    }
    namespace {
        void create_scene_directory() {
            const std::filesystem::path project_path = project::active::get_project_path();
            std::filesystem::path scenes_path = get_scenes_path();

            if ( !std::filesystem::exists(scenes_path) )
                std::filesystem::create_directory(scenes_path);
        }

        namespace {
            utl::vector<unsigned int> models_order;
            utl::vector<::content::model::model> loading_models;
            std::string loading_scene_name;
        }

        void load_detail(reader& detail_reader) {
            detail_reader.read(loading_scene_name);

        }

        void write_detail(const std::string& scene_uid, ::content::scene::scene& scene, const std::string& scene_name) {

            writer writer;
            writer.open(get_detail_path(scene_uid));
            writer.write(scene_name);

            std::string scene_detail_data;
            for ( auto& model : id_to_models )
                scene_detail_data += model::get_detail(model.second);
            writer.write(scene_detail_data, false);

            writer.flush();
            writer.close();
        }

        void write_base(const std::string& scene_uid, ::content::scene::scene& scene) {

            std::string scene_base_data;

            for ( auto& model : id_to_models )
                scene_base_data += model::get_base(model.second);

            writer writer;
            writer.open(get_base_path(scene_uid));

            writer.write(count);
            writer.write(scene_base_data, false);
            writer.write(model_positions.data(), model_positions.size());

            writer.flush();
            writer.close();
        }

        void load(reader& base_reader, reader& detail_reader) {
            unsigned int no_models;
            base_reader.read(no_models);
            models_order.clear();

            for ( unsigned int i = 0; i < no_models; ++i) {
                loading_models.emplace_back();
                model::load_base(base_reader, loading_models[i]);
                model::load_detail(detail_reader, loading_models[i]);
            }

            unsigned int no_components;
            base_reader.read(no_components);
            for ( unsigned int i = 0; i < no_components; ++i) {
                models_order.emplace_back();
                base_reader.read(models_order[i]);
            }
        }

        void save_scene(const std::string& scene_name, ::content::scene::scene& scene) {

            const std::string scene_uid = scene.get_uid();
            map_ids(scene);

            create_scene_directory();

            if ( !scene_exists(scene_uid) )
                write_base(scene_uid, scene);
            else std::cout << "Saving scene without base!" << std::endl;
            write_detail(scene_uid, scene, scene_name);
        }
    }

    void load_scene(const std::string& scene_uid) {
        auto base_path = get_base_path(scene_uid);
        auto detail_path = get_detail_path(scene_uid);

        reader base_reader;
        reader detail_reader;

        base_reader.open_and_store(base_path.string());
        detail_reader.open_and_store(detail_path.string());

        ::content::scene::scene scene;

        load_detail(detail_reader);
        load(base_reader, detail_reader);

        utl::vector<unsigned int> models_ids;

        for ( auto& model : loading_models)
            models_ids.emplace_back(::content::model::add_model(model));

        for ( unsigned int i = 0; i < models_order.size(); ++i )
            models_order[i] = models_ids[models_order[i]];

        scene._models = models_order;
        scene.set_uid(scene_uid);

        ::content::scene::create_empty_scene(loading_scene_name);
        *::content::scene::get_scene(loading_scene_name) = scene;

        loading_models.clear();
    }

    void save(writer& main_writer) {

        create_scene_directory();

        auto& scenes = ::content::scene::get_scenes();
        main_writer.write<unsigned int>(scenes.size());
        std::cout << "saving " << scenes.size() << " scenes" << std::endl;
        for ( auto& scene : scenes ) {
            main_writer.write(scene.second.get_uid());
            save_scene(scene.first, scene.second);
        }
    }
}
