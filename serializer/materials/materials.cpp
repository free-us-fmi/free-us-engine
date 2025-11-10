#include "materials.h"
#include "engine/materials/materials.h"
#include "engine/managers/MaterialManager.h"
#include "managers/TextureManager.h"
#include "project/active_project.h"
#include "textures.h"

namespace serializer::materials {

    namespace {
        constexpr const char* material_detail_extension = "mtd";
        constexpr const char* material_base_extension = "mtb";

        std::filesystem::path get_materials_path() {
            return project::active::get_project_path() / "materials";
        }

        std::filesystem::path get_material_path(const std::string& mat_uid, const char* extension) {
            assert(mat_uid.find("mat") != std::string::npos);
            std::filesystem::path material_path =  get_materials_path() / mat_uid;
            material_path.replace_extension(extension);

            return material_path;
        }

        std::filesystem::path get_detail_path(const std::string& mat_uid) {
            return get_material_path(mat_uid, material_detail_extension);
        }

        std::filesystem::path get_base_path(const std::string& mat_uid) {
            return get_material_path(mat_uid, material_base_extension);
        }

        bool material_exists(const std::string& mat_uid) {
            return std::filesystem::exists(get_material_path(mat_uid, material_detail_extension));
        }

        void create_material_directory() {
            const std::filesystem::path project_path = serializer::project::active::get_project_path();
            std::filesystem::path materials_path = get_materials_path();

            if ( !std::filesystem::is_directory(materials_path) )
                std::filesystem::remove(materials_path);

            if ( !std::filesystem::exists(materials_path) )
                std::filesystem::create_directory(materials_path);
        }

        struct material_data {
            std::string uid;
            utl::vector<unsigned char> data;
            unsigned int width = 0, height = 0, channels = 0;
        };

        struct material_detail {
            std::string name;
            std::unordered_map<::materials::material::texture_type, std::string> textures_uids;
        };
    }

    namespace {

        void retrieve_existing_textures(const std::string& uid, std::unordered_map<::materials::material::texture_type, material_data>& result) {

            auto existing_path = get_material_path(uid, material_base_extension);

            if ( !std::filesystem::exists(existing_path))
                return ;

            for ( unsigned int i = 0; i < ::materials::material::texture_type::count; ++i )
                result[static_cast<::materials::material::texture_type>(i)] = {};

            reader reader;
            reader.open_and_store(existing_path);

            for ( unsigned int i = 0; i < ::materials::material::texture_type::count; ++i ) {

                if ( !reader.is_in_range<unsigned int>() )
                    break;

                auto texture_type = static_cast<::materials::material::texture_type>(i);

                std::string _uid;
                reader.read(_uid);

                if ( _uid.empty() )
                    continue;

                unsigned int width, height, channels;
                reader.read(width); reader.read(height); reader.read(channels);

                result[texture_type].data.resize(width * height * channels);
                reader.read(result[texture_type].data.data());

                result[texture_type].uid = _uid;
                result[texture_type].width = width;
                result[texture_type].height = height;
                result[texture_type].channels = channels;
            }
        }

        material_detail retrieve_existing_detail(const std::string& uid) {

            material_detail detail;
            auto existing_path = get_material_path(uid, material_detail_extension);

            if ( !std::filesystem::exists(existing_path))
                return detail;

            reader reader;
            reader.open_and_store(existing_path);

            reader.read(detail.name);

            for ( unsigned int i = 0; i < ::materials::material::texture_type::count; ++i ) {
                if ( !reader.is_in_range<unsigned int>() )
                    break;

                auto texture_type = static_cast<::materials::material::texture_type>(i);

                std::string _uid;
                reader.read(_uid);

                detail.textures_uids[texture_type] = _uid;
            }

            return detail;
        }

        void save_material_base(const ::materials::material& material) {
            writer base_writer;

            std::unordered_map<::materials::material::texture_type, material_data> material_data;
            retrieve_existing_textures(material._uid, material_data);

            auto base_path = get_material_path(material._uid, material_base_extension);

            base_writer.open(base_path);

            for ( unsigned int i = 0; i < ::materials::material::texture_type::count; ++i ) {
                auto texture_type = static_cast<::materials::material::texture_type>(i);
                ::textures::texture_id texture_id = material.get_texture(texture_type);
                auto* texture = ::textures::get_texture(texture_id);

                if ( !texture ) {
                    base_writer.write(0);
                    continue;
                }

                if ( material_data[texture_type].uid == texture->get_uid() )
                    textures::save_to_tmp(texture->get_uid(), material_data[texture_type].data.data(),
                        material_data[texture_type].width, material_data[texture_type].height, material_data[texture_type].channels);

                std::string tex_data = textures::retrieve_from_tmp(texture->get_uid());

                base_writer.write(tex_data, false);
            }

            base_writer.flush();
            base_writer.close();
        }

        bool save_material_detail(const ::materials::material& material) {

            bool base_changed = false;
            material_detail detail = retrieve_existing_detail(material._uid);

            writer detail_writer;
            auto detail_path = get_material_path(material._uid, material_detail_extension);
            detail_writer.open(detail_path);
            detail_writer.write(material._name);

            for ( unsigned int i = 0; i < ::materials::material::texture_type::count; ++i ) {
                auto texture_type = static_cast<::materials::material::texture_type>(i);
                ::textures::texture_id texture_id = material.get_texture(texture_type);
                auto* texture = ::textures::get_texture(texture_id);

                std::string old_uid = detail.textures_uids[texture_type];

                if ( !texture ) {
                    if ( !old_uid.empty() )
                        base_changed = true;

                    detail_writer.write(0);
                    continue;
                }

                if ( texture->get_uid() != old_uid )
                    base_changed = true;

                detail_writer.write(texture->get_uid());
            }

            detail_writer.flush();
            detail_writer.close();

            return base_changed;
        }

        void save_material(::materials::material& material) {
            if ( save_material_detail(material) )
                save_material_base(material);
            else std::cout << "Saving material without base!" << std::endl;
        }

    }

    namespace {
        std::string load_detail(const std::string& material_uid) {

            material_detail detail = retrieve_existing_detail(material_uid);
            ::materials::AddMaterial(detail.name, material_uid);

            return detail.name;
        }

        void load_base(const std::string& material_uid, const std::string& material_name) {
            std::unordered_map<::materials::material::texture_type, material_data> material_data;
            retrieve_existing_textures(material_uid, material_data);

            auto* material = ::materials::GetMaterial(material_name);

            for ( unsigned int i = 0; i < ::materials::material::texture_type::count; ++i ) {
                auto texture_type = static_cast<::materials::material::texture_type>(i);

                if ( !material_data[texture_type].uid.empty() )
                    material->set_texture_from_data(material_data[texture_type].uid, texture_type, material_data[texture_type].data.data(),
                        material_data[texture_type].width, material_data[texture_type].height, material_data[texture_type].channels);
            }
        }

        void load_material(const std::string& material_uid) {
            std::string material_name = load_detail(material_uid);
            load_base(material_uid, material_name);
        }
    }

    void save(writer& main_writer) {
        create_material_directory();
        auto& materials = ::materials::get_materials();
        main_writer.write(materials.allocated());
        for ( auto& material : materials ) {
            main_writer.write(material._uid);
            save_material(material);
        }
    }

    void load(reader& main_reader) {
        unsigned int count;
        main_reader.read(count);

        while (count--) {
            std::string material_uid;
            main_reader.read(material_uid);
            load_material(material_uid);
        }
    }


}